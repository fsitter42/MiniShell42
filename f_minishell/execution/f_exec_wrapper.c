/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exec_wrapper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:02:15 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 01:13:04 by slambert         ###   ########.fr       */
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

void	f_close_child(int *pipe_fd, int *prev_fd, t_cmd *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
	}
}

void	f_setup_pipe_fds(t_data *data, t_cmd *cmd, int *prev_fd, int *pipe_fd)
{
	if (*prev_fd != -1)
	{
		if (dup2(*prev_fd, STDIN_FILENO) == -1)
			f_dup_error(data, cmd, pipe_fd, prev_fd);
		close(*prev_fd);
		*prev_fd = -1;
	}
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			f_dup_error(data, cmd, pipe_fd, prev_fd);
		close(pipe_fd[1]);
		pipe_fd[1] = -1;
	}
}

void	f_setup_cmd_fds(t_data *data, t_cmd *cmd, int *pipe_fd, int *prev_fd)
{
	if (cmd->in_fd != -1)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			f_dup_error(data, cmd, pipe_fd, prev_fd);
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd != -1)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			f_dup_error(data, cmd, pipe_fd, prev_fd);
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
}
