/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exec_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 23:12:59 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 01:06:22 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	f_close_cmd_fds(t_cmd *cmd);

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
	data->should_exit = 1;
	return (-1);
}

void	f_dup_error(t_data *data, t_cmd *cmd, int *pipe_fd, int *prev_fd)
{
	f_print_error("dup2", strerror(errno));
	f_close_cmd_fds(cmd);
	f_close_child(pipe_fd, prev_fd, cmd);
	sfbf_free_all(data);
	exit(88);
}

static void	f_close_cmd_fds(t_cmd *cmd)
{
	if (cmd->in_fd != -1)
	{
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd != -1)
	{
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
}

int	f_redir_error(int saved_fds[2], int fd, char *err)
{
	f_print_error(err, strerror(errno));
	if (fd != -1)
		close(fd);
	if (saved_fds[0] != -1)
	{
		close(saved_fds[0]);
		saved_fds[0] = -1;
	}
	if (saved_fds[1] != -1)
	{
		close(saved_fds[1]);
		saved_fds[1] = -1;
	}
	return (-1);
}
