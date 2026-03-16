/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:03:24 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_pwd(t_data *data, char **args)
{
	int	ret;

	ret = EXIT_SUCCESS;
	if (!data || !args || !args[0])
		return (EXIT_FAILURE);
	//ret = f_print_env(data->env->envp_lst);
	return (ret);
}
