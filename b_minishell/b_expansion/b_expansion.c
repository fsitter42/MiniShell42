/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_expansion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/04/08 16:27:05 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	append_expanded_char(char **out, char *word, int *state,
		t_data *data)
{
	int	ret;

	if (consume_syntactic_quote(word[state[0]], &state[1]) == 1)
		return (RET_OK);
	if (word[state[0]] == '$' && state[1] != IN_SINGLE_Q && word[state[0] + 1])
	{
		if (word[state[0] + 1] == '?')
			ret = expand_dollar_question(out, &state[0], data);
		else
			ret = append_env_var(out, word, &state[0], data);
		if (ret != RET_OK)
			return (ret);
		return (RET_OK);
	}
	*out = append_char(*out, word[state[0]]);
	if (!*out)
		return (ERROR_HARD);
	return (RET_OK);
}

/*
 *	loops through the word, consumes syntactic quotes and expands
 *	vars (either env vars or $?)
 *  state[0] holds i
 *  state[1] holds quote_status
 */
static char	*expand_word_one_pass(char *word, t_data *data, int *ret_status)
{
	char	*out;
	int		state[2];

	*ret_status = RET_OK;
	state[1] = DEFAULT_Q;
	out = ft_strdup("");
	if (!out)
	{
		*ret_status = ERROR_HARD;
		return (NULL);
	}
	state[0] = -1;
	while (word[++state[0]])
	{
		*ret_status = append_expanded_char(&out, word, state, data);
		if (*ret_status != RET_OK)
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
	int		ret;

	if (list_elem->str && list_elem->str[0] == '~')
	{
		ret = expand_home_dir(list_elem, data->env->envp_updated);
		if (ret != RET_OK)
			return (ret);
	}
	expanded = expand_word_one_pass(list_elem->str, data, &ret);
	if (!expanded)
		return (ret);
	free(list_elem->str);
	list_elem->str = expanded;
	return (RET_OK);
}

int	expansion(t_token *list, t_data *data)
{
	int	ret;

	while (list)
	{
		if (list->type == WORD)
		{
			ret = expand_single_word(list, data);
			if (ret != RET_OK)
				return (ret);
		}
		list = list->next;
	}
	return (RET_OK);
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
 *	ODO differenzieren zwischen leerer variable und malloc fail
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