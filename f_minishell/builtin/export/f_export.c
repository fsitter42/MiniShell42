/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:26:09 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:03:05 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_export(t_data *data, char **args)
{
	int	ret;
	int	i;

	ret = EXIT_SUCCESS;
	if (!data || !args || !args[0])
		return (EXIT_FAILURE);
	if (!args[1])
		return (f_print_export(data->env->envp_lst));
	i = 1;
	while (args[i])
	{
		if (ft_strlen(args[i]) > MAX_ENV_LEN)
		{
			f_print_error(args[i], "argument too long");
			ret = EXIT_FAILURE;
		}
		else
			ret = f_export_with_key(data->env->envp_lst, args[i]);
		i++;
	}
	ret = f_update_envp(data->env);
	return (ret);
}

/*
 *   return value = g_last_exit_code = f_export() aufruden
 *	args = der command inklusve argumente
 */