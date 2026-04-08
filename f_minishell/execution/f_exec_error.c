/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pipe_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 23:12:59 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/08 23:24:55 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_pipe_error(t_data *data, t_cmd *cmd, int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	f_print_error("pipe", strerror(errno));
	data->last_exit_code = 1;
	return (-1);
}

int	f_fork_error(t_data *data, t_cmd *cmd, int pipe_fd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	f_print_error("fork", strerror(errno));
	data->last_exit_code = 1;
	return (-1);
}
