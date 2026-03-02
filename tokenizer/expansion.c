/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/02 16:40:14 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *extract_home_path_from_envp(char **envp)
{
    char *home;
    int i;
    
    home = NULL;
    i = -1;
    while (envp[++i])
    {
        if (ft_strncmp(envp[i], "HOME=", 5) == 0)
            return ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5) ;
    }
    return NULL;
}

//TODO add "= after var_name"
char *extract_var_from_envp(char *var_name, char **envp)
{
    char *value;
    int i;
    int len_var_name;
    int len_total;
    
   // var_name
    i = -1;
    len_var_name = ft_strlen(var_name);
    while (envp[++i])
    {
        if (ft_strncmp(envp[i], var_name, len_var_name) == 0)
        {
            len_total = ft_strlen(envp [i]);
            return ft_substr(envp[i], len_var_name, len_total - len_var_name);
        }
    }
    return ft_strdup("");
}

char *replace_char_with_expandable(char *original, char char_to_expand, char *expandable)
{
    
    char *temp;
    int i_src;
    int i_dst;
    
    i_src = 0;
    i_dst = 0;
    
    temp = ft_calloc(ft_strlen(original) + ft_strlen(expandable) + 1, sizeof(char));
    
    while (original[i_src]) {
        if (original[i_src] == char_to_expand) 
        {
            ft_strlcpy(&temp[i_dst], expandable, ft_strlen(expandable) + 1);
            i_dst += ft_strlen(expandable);
            i_src++;
            break;
        }
        temp[i_dst++] = original[i_src++];
    }
    while (original[i_src])
        temp[i_dst++] = original[i_src++];
    temp[i_dst] = '\0';
    return temp;
}

//char *replace_word_with_expandable(char *original, char char_to_expand, char *expandable)

int	quote_handler(int quote_status, char c)
{
	if (quote_status == DEFAULT_QUOTE)
	{
		if (c == '\'')
			return IN_SINGLE_QUOTES;
		if (c == '\"')
			return IN_DOUBLE_QUOTES;
	}
	else if ((quote_status == IN_SINGLE_QUOTES && c == '\'') || (quote_status == IN_DOUBLE_QUOTES && c == '\"'))
			return DEFAULT_QUOTE;
	return quote_status;
}

void expand_home_dir (t_token *list_elem, char **envp)
{
    char *home;
    char *temp;
    
    home = NULL;
    home = extract_home_path_from_envp(envp);
    //if (!home)
    //ERROR
    //printf("home is %s\n", home);
    temp = replace_char_with_expandable(list_elem->str, '~', home);
    //if (!temp)
    //ERROR
    free(home);
    free(list_elem->str);
    list_elem->str = temp;
    //printf("temp is %s\n", temp);        
}

/*
*  helper for the case that the $ is not in the beginning but in the middle of a token
*  in that case we have to strjoin the first string until the $ (excluding) with the var value
*
*/
char *return_helper(char *str, char *temp)
{
    char *dollar_position;
    int length_to_keep;
    
    dollar_position = ft_strchr(str, '$');
    length_to_keep = dollar_position - str;
    
    //possible todo das nicht in einer zeile
    return ft_strjoin(ft_substr(str, 0, length_to_keep), temp);
}

//TODO: var_name ohne klammern am ende (???)
/*  replaces the first occurrence of $ with the value
*/
char *replace_var_with_content(char *str, char **envp)
{
    int i;
    char *var_name_without_istgleich;
    char *var_name_with_istgleich;
    char *temp;

    temp = ft_strdup("");
    if (!temp)
        return (NULL);
    i = -1;
    while (str[++i])
    {
        if (str[i] == '$')
        {
            var_name_without_istgleich = ft_substr(str, i + 1, ft_strlen(str + 1));
            if (!var_name_without_istgleich)
                return (temp);
            var_name_with_istgleich = ft_strjoin(var_name_without_istgleich, "=");
            if (!var_name_with_istgleich)
            {
                free(var_name_without_istgleich);
                return (temp);
            }
            printf("var_name is %s\n", var_name_with_istgleich);
            //for variables in envp
            free(temp);
            temp = extract_var_from_envp(var_name_with_istgleich, envp);
            free(var_name_without_istgleich);
            free(var_name_with_istgleich);
            break;
        }
    }
    if (str[0] == '$')
        return (temp);
    else
        return return_helper(str, temp);
}

//replaces all occurences of a variable with the dedicated value
//this can happen multiple times so we loop
void expand_variable (t_token *list_elem, char **envp)
{
    int i;
    char *temp;
    char *expanded;

    i = 0;
    while(list_elem->str && list_elem->str[i])
    {
        if (list_elem->str[i] == '$')
        {
            temp = ft_strdup(list_elem->str);
            //ERROR
            free(list_elem->str);

            expanded = replace_var_with_content(temp, envp);
            free(temp);
            list_elem->str = expanded;
            //if (!list_elem->status)
            //error
            i = 0;
            continue;
        }
        i++;   
    }
}

/*
*  RULES: 
*  - single quotes contain the original (literal) value
*  - double quotes allow the expansion of variables
*
*  1. expand variables
*  2. expand ~ (do we need that?)
*  3. expand $?
*/
void expand_single_word(t_token *list_elem, char **envp)
{
    int i;
    int quote_status;

    quote_status = DEFAULT_QUOTE;
    i = -1;
    while (list_elem->str[++i])
    {
        quote_status = quote_handler(quote_status, list_elem->str[i]);
        if (list_elem->str[i] == '~' && i == 0 && quote_status == DEFAULT_QUOTE)
            expand_home_dir(list_elem, envp);
        if (list_elem->str[i] == '$' && quote_status != IN_SINGLE_QUOTES)
            expand_variable(list_elem, envp);
    }
}

/*
*  loops though all words and executes the expand_word function
*/
void expansion (t_token *list, char **envp)
{
    while (list)
    {
        if (list->type == WORD)
            expand_single_word(list, envp);
        list = list->next;
    } 
}