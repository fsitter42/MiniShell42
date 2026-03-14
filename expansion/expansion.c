/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:33:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/14 16:59:35 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// getenv would be allowed
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

/*
 *	possible TODO: don't return malloced string then we don't have to free
 *	in resolve_env_var
 *
 */
char	*extract_var_from_envp(char *var_name)
{
	char *ret;

	ret = getenv(var_name);
	if (!ret)
		return (ft_strdup(""));
	return (ft_strdup(ret));
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
 *	TODO change to getenv
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

static int	get_var_name_len(char *word, int i)
{
	int	var_name_len;

	var_name_len = 0;
	while (word[i + 1 + var_name_len] && is_var_char(word[i + 1
			+ var_name_len]))
		var_name_len++;
	return (var_name_len);
}

static int	resolve_env_var_value(char *var_name, char **value)
{
	*value = extract_var_from_envp(var_name);
	if (!*value)
		return (1);
	return (0);
}

static int	append_env_var(char **out, char *word, int *i)
{
	int		var_name_len;
	char	*var_name;
	char	*value;

	var_name_len = get_var_name_len(word, *i);
	if (var_name_len == 0)
	{
		*out = append_char(*out, '$');
		if (!*out)
			return (1);
		(*i)++;
		return (0);
	}
	var_name = ft_substr(word, *i + 1, var_name_len);
	if (!var_name)
		return (1);
	if (resolve_env_var_value(var_name, &value) == 1)
		return (free(var_name), 1);
	*out = append_str(*out, value);
	free(value);
	free(var_name);
	if (!*out)
		return (1);
	*i += 1 + var_name_len - 1;
	return (0);
}

/*
 *	expands $? to the exit code number
 */
static int	expand_dollar_question(char **out, int *i)
{
	char	*temp;

	temp = ft_itoa(g_last_exit_code);
	if (!temp)
		return (1);
	*out = append_str(*out, temp);
	free(temp);
	if (!*out)
		return (1);
	*i += 1;
	return (0);
}

void	add_to_list(t_token *list, t_token *new)
{
	while (list->next)
		list = list->next;
	list->next = new;
}

/*
 * removes a quote character when that quote changes the quote state
 * (opens or closes the current mode)
 * returns 1 if a quote has been consumed
 */
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

static int	append_expanded_char(char **out, char *word, int *i,
		int *quote_status)
{
	if (consume_syntactic_quote(word[*i], quote_status) == 1)
		return (0);
	if (word[*i] == '$' && *quote_status != IN_SINGLE_QUOTES)
	{
		if (word[*i + 1] == '?')
			return (expand_dollar_question(out, i));
		return (append_env_var(out, word, i));
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
char	*expand_word_one_pass(char *word)
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
		if (append_expanded_char(&out, word, &i, &quote_status) == 1)
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
 *  1. variables
 *  2. ~
 *  3. $? TODO
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
	expanded = expand_word_one_pass(list_elem->str);
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