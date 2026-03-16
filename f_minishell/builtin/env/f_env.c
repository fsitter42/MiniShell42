/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:37:21 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:02:55 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_env(t_data *data, char **args)
{
	int	ret;

	ret = EXIT_SUCCESS;
	if (!data || !args || !args[0])
		return (EXIT_FAILURE);
	ret = f_print_env(data->env->envp_lst);
	return (ret);
}

int	f_print_env(t_envl *list_head)
{
	if (!list_head)
		return (EXIT_FAILURE);
	while (list_head)
	{
		if (list_head->val != NULL)
		{
			ft_putstr_fd(list_head->key, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(list_head->val, 1);
			ft_putchar_fd('\n', 1);
		}
		list_head = list_head->next;
	}
	return (EXIT_SUCCESS);
}
