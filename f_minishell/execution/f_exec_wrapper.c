/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exec_wrapper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:02:15 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/08 23:24:40 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_pipeline_wrapper(t_data *data)
{
	int		ret;
	t_cmd	*cmd;
	int		pipe_fd[2];

	cmd = data->cmds;
	ret = f_exec_pipeline(data, cmd, pipe_fd);
	return (ret);
}

void	f_close_pipe(t_cmd *cmd, int pipe_fd[2], int *prev_fd)
{
	if (cmd->next)
	{
		if (*prev_fd != -1)
			close(*prev_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	f_close_child(int *pipe_fd, int prev_fd, t_cmd *cmd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}
