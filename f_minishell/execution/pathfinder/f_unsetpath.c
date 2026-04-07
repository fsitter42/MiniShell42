/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_unsetpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:41:07 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/07 14:57:56 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_path_set(t_data *data)
{
	t_envl	*current;

	current = data->env->envp_lst;
	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 5) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}
