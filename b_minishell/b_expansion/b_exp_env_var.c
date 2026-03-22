/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_env_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:14 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 14:40:55 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *	possible TODO: don't return malloced string then we don't have to free
 *	in resolve_env_var
 *
 */
char	*extract_var_from_envp(char *var_name)
{
	char	*ret;

	ret = getenv(var_name);
	if (!ret)
		return (ft_strdup(""));
	return (ft_strdup(ret));
}

int	resolve_env_var_value(char *var_name, char **value)
{
	*value = extract_var_from_envp(var_name);
	if (!*value)
		return (1);
	return (0);
}

int	append_env_var(char **out, char *word, int *i)
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
