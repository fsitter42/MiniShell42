/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_expansion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 10:52:26 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* heredoc mode is here bc the syntactic quotes must not be removed on a
 * delimiter. is heredoc mode dont consume quotes
 */
static int	append_expanded_char(char **out, char *word, t_exp_struct *texp,
		t_data *data)
{
	int	ret;

	if (!texp->heredoc_mode && consume_syntactic_quote(word[texp->i],
			&texp->quote_status) == 1)
		return (RET_OK);
	if (word[texp->i] == '$' && word[texp->i + 1] && (texp->heredoc_mode
			|| texp->quote_status != IN_SINGLE_Q))
	{
		if (word[texp->i + 1] == '?')
			ret = expand_dollar_question(out, &texp->i, data);
		else
			ret = append_env_var(out, word, &texp->i, data);
		if (ret != RET_OK)
			return (ret);
		return (RET_OK);
	}
	*out = append_char(*out, word[texp->i]);
	if (!*out)
		return (ERROR_HARD);
	return (RET_OK);
}

char	*expand_word_one_pass(char *word, t_data *data, int heredoc_mode,
		int *ret_status)
{
	char			*out;
	t_exp_struct	tex;

	*ret_status = RET_OK;
	out = ft_strdup(""); // NULL;// TODO F B leak glaube bei hd
	if (!out)
	{
		*ret_status = ERROR_HARD;
		return (NULL);
	}
	tex.i = -1;
	tex.quote_status = DEFAULT_Q;
	tex.heredoc_mode = heredoc_mode;
	while (word[++tex.i])
	{
		*ret_status = append_expanded_char(&out, word, &tex, data); //TODO failen lassen exit code
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

	if (list_elem->str && !ft_strncmp(list_elem->str, "~",
			ft_strlen(list_elem->str)))
	{
		ret = expand_home_dir(list_elem, data->env->envp_updated);
		if (ret != RET_OK)
			return (ret);
	}
	expanded = expand_word_one_pass(list_elem->str, data, FALSE, &ret);
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