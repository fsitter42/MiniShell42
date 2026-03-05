/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/05 15:28:40 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_home_path_from_envp(char **envp)
{
	char	*home;
	int		i;

	home = NULL;
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "HOME=", 5) == 0)
			return (ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5));
	}
	return (NULL);
}

char	*extract_var_from_envp(char *var_name, char **envp)
{
	char	*value;
	int		i;
	int		len_var_name;
	int		len_total;

	i = -1;
	len_var_name = ft_strlen(var_name);
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], var_name, len_var_name) == 0)
		{
			len_total = ft_strlen(envp[i]);
			return (ft_substr(envp[i], len_var_name, len_total - len_var_name));
		}
	}
	return (NULL);
}

// replaces a singular character with a string
// mem safe on error
char	*replace_char_with_expandable(char *original, char char_to_expand,
		char *expandable)
{
	char	*temp;
	int		i_src;
	int		i_dst;
	int		size;

	i_src = 0;
	i_dst = 0;
	size = ft_strlen(original) - 1 + ft_strlen(expandable) + 1;
	temp = ft_calloc(size, sizeof(char));
	if (!temp)
		return (NULL);
	while (original[i_src])
	{
		if (original[i_src] == char_to_expand)
		{
			ft_strlcpy(&temp[i_dst], expandable, ft_strlen(expandable) + 1);
			i_dst += ft_strlen(expandable);
			i_src++;
			break ;
		}
		temp[i_dst++] = original[i_src++];
	}
	while (original[i_src])
		temp[i_dst++] = original[i_src++];
	// temp[i_dst] = '\0';
	return (temp);
}

int	quote_handler(int quote_status, char c)
{
	if (quote_status == DEFAULT_QUOTE)
	{
		if (c == '\'')
			return (IN_SINGLE_QUOTES);
		if (c == '\"')
			return (IN_DOUBLE_QUOTES);
	}
	else if ((quote_status == IN_SINGLE_QUOTES && c == '\'')
			|| (quote_status == IN_DOUBLE_QUOTES && c == '\"'))
		return (DEFAULT_QUOTE);
	return (quote_status);
}

/*
 *  memory safe on error
 */
int	expand_home_dir(t_token *list_elem, char **envp)
{
	char	*home;
	char	*temp;

	home = NULL;
	home = extract_home_path_from_envp(envp);
	if (!home)
		return (1);
	temp = replace_char_with_expandable(list_elem->str, '~', home);
	if (!temp)
		return (free(home), 1);
	free(home);
	free(list_elem->str);
	list_elem->str = temp;
}

/*
 *  RULES:
 *  - single quotes contain the original (literal) value
 *  - double quotes (or no quotes) allow the expansion of variables
 *
 *  1. expand variables
 *  2. expand ~
 *  3. expand $?
 *
 *  returns 1 on error
 */
int	expand_single_word(t_token *list_elem, char **envp)
{
	char	*expanded;

	if (list_elem->str && list_elem->str[0] == '~')
	{
		if (expand_home_dir(list_elem, envp) == 1)
			return (1);
	}
	expanded = expand_word_one_pass(list_elem->str, envp);
	if (!expanded)
		return (1);
	free(list_elem->str);
	list_elem->str = expanded;
	return (0);
}

static char	*append_str(char *dst, char *src)
{
	char	*new_str;

	new_str = ft_strjoin(dst, src);
	free(dst);
	return (new_str);
}

static char	*append_char(char *dst, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_str(dst, tmp));
}

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	consume_syntactic_quote(char c, int *quote_status)
{
	int	next_quote_status;

	next_quote_status = quote_handler(*quote_status, c);
	if ((c == '\'' || c == '"') && next_quote_status != *quote_status)
	{
		*quote_status = next_quote_status;
		return (1);
	}
	return (0);
}

static int	get_var_len(char *word, int i)
{
	int	var_len;

	var_len = 0;
	while (word[i + 1 + var_len] && is_var_char(word[i + 1 + var_len]))
		var_len++;
	return (var_len);
}

static int	append_env_value(char **out, char *word, int *i, char **envp)
{
	int		var_len;
	char	*var_name;
	char	*var_with_equal;
	char	*value;

	var_len = get_var_len(word, *i);
	if (var_len == 0)
	{
		*out = append_char(*out, '$');
		if (!*out)
			return (1);
		(*i)++;
		return (0);
	}
	var_name = ft_substr(word, *i + 1, var_len);
	if (!var_name)
		return (1);
	var_with_equal = ft_strjoin(var_name, "=");
	free(var_name);
	if (!var_with_equal)
		return (1);
	value = extract_var_from_envp(var_with_equal, envp);
	free(var_with_equal);
	if (!value)
		value = ft_strdup("");
	if (!value)
		return (1);
	*out = append_str(*out, value);
	free(value);
	if (!*out)
		return (1);
	*i += 1 + var_len;
	return (0);
}

static int	append_dollar_question(char **out, int *i)
{
	*out = append_str(*out, "$?");
	if (!*out)
		return (1);
	*i += 2;
	return (0);
}

char	*expand_word_one_pass(char *word, char **envp)
{
	int		i;
	int		quote_status;
	char	*out;

	quote_status = DEFAULT_QUOTE;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (consume_syntactic_quote(word[i], &quote_status))
		{
			i++;
			continue ;
		}
		if (word[i] == '$' && quote_status != IN_SINGLE_QUOTES)
		{
			if (word[i + 1] == '?')
			{
				if (append_dollar_question(&out, &i) == 1)
					return (free(out), NULL);
				continue ;
			}
			if (append_env_value(&out, word, &i, envp) == 1)
				return (free(out), NULL);
			continue ;
		}
		out = append_char(out, word[i]);
		if (!out)
			return (NULL);
		i++;
	}
	return (out);
}

/*
 *  loops though all words and executes the expand_word function
 *  returns 1 on error
 */
int	expansion(t_token *list, char **envp)
{
	while (list)
	{
		if (list->type == WORD)
		{
			if (expand_single_word(list, envp) == 1)
				return (1);
		}
		list = list->next;
	}
	return (0);
}

//OLD STUFF
/*
 *  Builds a new word where the first expandable variable is replaced.
 *  Result format: [prefix before '$'] + [temp expanded value]
	+ [suffix after var name].
 *  valid ariable name chars are [A-Z | a-z | 0-9 | _].
 *  RETURN: newly allocated string, or NULL on allocation failure.
 */
/* char	*string_creation_helper(char *str, char *temp)
{
	char	*dollar_pos;
	char	*result;
	int		var_len;
	int		i;
	int		j;

	dollar_pos = ft_strchr(str, '$');
	var_len = 0;
	while (dollar_pos[1 + var_len] && (ft_isalnum(dollar_pos[1 + var_len])
			|| dollar_pos[1 + var_len] == '_'))
		var_len++;
	result = ft_calloc(ft_strlen(str) - 1 - var_len + ft_strlen(temp) + 1,
			sizeof(char));
	if (!result)
		return (free(temp), NULL);
	i = 0;
	j = 0;
	while (&str[i] < dollar_pos)
		result[j++] = str[i++];
	i++;
	ft_strlcpy(&result[j], temp, ft_strlen(temp) + 1);
	j += ft_strlen(temp);
	while (str[i + var_len])
		result[j++] = str[i++ + var_len];
	free(temp);
	return (result);
} */

// TODO: var_name ohne klammern am ende (???)
// TODO: kann var_name_without_istgleich durch temp ersetzt werden?
// TODO: ev. ohne return sondern mit pointer und return wieviele chars,
// dann muss ich i nicht auf 0 setzen in der aufrufenden funktion?
/*  replaces the first occurrence of $ with the value
 *   return NULL on error
 *   memory safe on error
 */
/* char	*replace_var_with_content(char *str, char **envp)
{
	int		i;
	char	*var_name_without_istgleich;
	char	*var_name_with_istgleich;
	char	*temp;
	int		var_len;

	temp = ft_strdup("");
	if (!temp)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			var_len = 0;
			while (str[i + 1 + var_len] && (ft_isalnum(str[i + 1 + var_len])
					|| str[i + 1 + var_len] == '_'))
				var_len++;
			if (var_len == 0) // $ followed by non-variable char zb $/
				continue ;
			var_name_without_istgleich = ft_substr(str, i + 1, var_len);
			if (!var_name_without_istgleich)
				return (free(temp), NULL);
			var_name_with_istgleich = ft_strjoin(var_name_without_istgleich,
					"=");
			if (!var_name_with_istgleich)
				return (free(temp), free(var_name_without_istgleich), NULL);
			printf("var_name is %s\n", var_name_with_istgleich);
			free(temp);
			temp = extract_var_from_envp(var_name_with_istgleich, envp);
			free(var_name_without_istgleich);
			free(var_name_with_istgleich);
			if (!temp)
				return (NULL);
			break ;
		}
	}
	return (string_creation_helper(str, temp));
} */

// replaces all occurences of a variable with the dedicated value
// TODO: this can happen multiple times so we have to loop
// e.g. $USER$USER
/* int	expand_variable(t_token *list_elem, char **envp)
{
	int		i;
	char	*temp;
	char	*expanded;

	// expanded = NULL;
	i = 0;
	while (list_elem->str && list_elem->str[i])
	{
		if (list_elem->str[i] == '$')
		{
			temp = ft_strdup(list_elem->str);
			if (!temp)
				return (1);
			free(list_elem->str);
			// free (expanded);
			expanded = replace_var_with_content(temp, envp);
			free(temp);
			if (!expanded)
				return (1);
			list_elem->str = expanded;
			i = 0;
			continue ;
		}
		i++;
	}
	return (0);
} */

/* void	handle_dollar_question(t_token *list_elem, char **envp)
{
	printf("I AM A PLACEHOLDER\n");
} */
