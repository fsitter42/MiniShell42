/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:26:09 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/28 12:11:00 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	f_check_arg(char *arg)
{
	if (!arg || !*arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (f_print_error(arg, "not a valid identifier"), 1);
	if (ft_strlen(arg) > MAX_ENV_LEN)
		return (f_print_error(arg, "argument too long"), 1);
	return (0);
}

int	f_export(t_data *data, char **args)
{
	int	i;
	int	ret;

	if (!data || !args)
		return (EXIT_FAILURE);
	if (!args[1])
		return (f_print_export(data->env->envp_lst));
	ret = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (f_check_arg(args[i]))
			ret = EXIT_FAILURE;
		else if (f_export_with_key(data->env->envp_lst, args[i], data))
		{
			data->should_exit = 1; //frage an bert wenn ich hier rein komme war ein malloc error kann man das dann so setzen?? damit er nach den fehler alles beendet?
			ret = EXIT_FAILURE;
		}
		i++;
	}
	f_update_envp(data->env, data);
	return (ret);
}
