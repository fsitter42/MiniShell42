/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_wait.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:22:41 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/15 12:40:12 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_command_count(t_data *data)
{
	int		cc;
	t_cmd	*current;

	cc = 0;
	current = data->cmds;
	while (current)
	{
		cc++;
		current = current->next;
	}
	return (cc);
}
