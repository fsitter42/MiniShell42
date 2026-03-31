/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_env_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:14 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 19:09:20 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *	possible TODO: don't return malloced string then we don't have to free
 *	in resolve_env_var
 *  TODO ned getenv sondern eigene funktion die auf data->env_copied extrahiert
 *  ret = f_get_env_val
 */
char	*extract_var_from_envp(char *var_name)
{
	char	*ret;

	ret = getenv(var_name);
	if (!ret)
		return (ft_strdup(""));
	return (ft_strdup(ret));
}

int	resolve_env_var_value(char *var_name, char **value, t_data *data)
{
	*value = f_get_env_val(data->env->envp_lst, var_name, data);
	if (!*value)
	{
		if (data->should_exit)
			return (ERROR_HARD);
		*value = ft_strdup("");
		if (!*value)
			return (ERROR_HARD);
	}
	return (RET_OK);
}

static int	append_literal_dollar(char **out)
{
	*out = append_char(*out, '$');
	if (!*out)
		return (ERROR_HARD);
	return (RET_OK);
}

int	append_env_var(char **out, char *word, int *i, t_data *data)
{
	int		var_name_len;
	int		ret;
	char	*var_name;
	char	*value;

	var_name_len = get_var_name_len(word, *i);
	if (var_name_len == 0)
		return (append_literal_dollar(out));
	var_name = ft_substr(word, *i + 1, var_name_len);
	if (!var_name)
		return (ERROR_HARD);
	ret = resolve_env_var_value(var_name, &value, data);
	if (ret != RET_OK)
		return (free(var_name), ret);
	*out = append_str(*out, value);
	free(value);
	free(var_name);
	if (!*out)
		return (ERROR_HARD);
	*i += 1 + var_name_len - 1;
	return (RET_OK);
}
