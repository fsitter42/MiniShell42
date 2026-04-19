/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_env_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:14 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 15:04:39 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	resolve_env_var_value(char *var_name, char **value, t_data *data)
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

// failen lassen
int	append_env_var(char **out, char *word, int *i, t_data *data)
{
	int		var_name_len;
	int		ret;
	char	*var_name;
	char	*value;

	var_name_len = get_var_name_len(word, *i);
	if (var_name_len == 0)
		return (append_literal_dollar(out)); // TODO wie kann ich das failen lassen also welchen befehl muss ich ausfuehren dass ich hier hinkomme glaube auch exitcode 0 keine mesage
	var_name = ft_substr(word, *i + 1, var_name_len); // TODO geliche wie darueber
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
