/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/02 13:28:26 by slambert         ###   ########.fr       */
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

void expand_home_dir (t_token *list_elem, char **envp)
{
    char *home;
    char *temp;
    int i;
    int j;
    int size_new_string;
    
    home = NULL;
    home = extract_home_path_from_envp(envp);
    //if (!home)
    //ERROR
    printf("home is %s\n", home);
    size_new_string = ft_strlen(list_elem->str) + ft_strlen(home);
    temp = ft_calloc(size_new_string + 1, sizeof(char));
    //if (!temp)
    //error
    i = 0;
    j = -1;
    while (i < size_new_string)
    {
        if (list_elem->str[i] != '~')
            temp[i] = list_elem->str[i];
        else
        {
            while (++j < ft_strlen(home))
            {
                temp[i] = home[j];
                i++;
            }
        }
        i++;
    }
    free(home);
    free(list_elem->str);
    list_elem->str = temp;
    //printf("temp is %s\n", temp);        
}

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
        if (list_elem->str[i] == '~' && quote_status == DEFAULT_QUOTE)
            expand_home_dir(list_elem, envp);
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