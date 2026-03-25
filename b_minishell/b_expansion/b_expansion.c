/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_expansion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/25 12:57:09 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	append_expanded_char(char **out, char *word, int *i,
		int *quote_status, t_data *data)
{
	if (consume_syntactic_quote(word[*i], quote_status) == 1)
		return (0);
	if (word[*i] == '$' && *quote_status != IN_SINGLE_QUOTES)
	{
		if (word[*i + 1] == '?')
			return (expand_dollar_question(out, i, data));
		return (append_env_var(out, word, i, data));
	}
	*out = append_char(*out, word[*i]);
	if (!*out)
		return (1);
	return (0);
}

/*
 *	loops through the word, consumes syntactic quotes and expands
 *	vars (either env vars or $?)
 */
char	*expand_word_one_pass(char *word, t_data *data)
{
	int		i;
	int		quote_status;
	char	*out;

	quote_status = DEFAULT_QUOTE;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	i = -1;
	while (word[++i])
	{
		if (append_expanded_char(&out, word, &i, &quote_status, data) == 1)
			return (free(out), NULL);
	}
	return (out);
}

/*
 *  RULES:
 *  - single quotes contain the original (literal) value (no expansion)
 *  - double quotes (or no quotes) allow the expansion of variables
 *
 * 	we have to expand 3 things:
 *  1. env variables
 *  2. ~ (home dir)
 *  3. $?
 *
 *  returns 1 on error
 */
int	expand_single_word(t_token *list_elem, t_data *data)
{
	char	*expanded;

	if (list_elem->str && list_elem->str[0] == '~')
	{
		if (expand_home_dir(list_elem, data->env->envp_updated) == 1)
			return (1);
	}
	expanded = expand_word_one_pass(list_elem->str, data);
	if (!expanded)
		return (1);
	free(list_elem->str);
	list_elem->str = expanded;
	return (0);
}

/*
 *  loops trough all words and executes the expand_word function
 *  returns 1 on error
 */
int	expansion(t_token *list, t_data *data)
{
	while (list)
	{
		if (list->type == WORD)
		{
			if (expand_single_word(list, data) == 1)
				return (1);
		}
		list = list->next;
	}
	return (0);
}

/* static int	append_env_var(char **out, char *word, int *i, char **envp)
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
	*i += 1 + var_len - 1;
		// bc change in incrementation logic in expand_word_one_pass
	return (0);
} */

// FRIDO brauchst du das für set

/* char	*extract_var_from_envp(char *var_name)
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
} */

/*
 *	TODO differenzieren zwischen leerer variable und malloc fail
 *	getenv!
 */
/* static int	resolve_env_var_value(char *var_name, char **value, char **envp)
{
	char	*var_with_equal;

	var_with_equal = ft_strjoin(var_name, "=");
	free(var_name);
	if (!var_with_equal)
		return (1);
	*value = extract_var_from_envp(var_with_equal, envp);
	printf("val is %s\n", *value);
	free(var_with_equal);
	if (!*value)
		*value = ft_strdup("");
	if (!*value)
		return (1);
	return (0);
} */