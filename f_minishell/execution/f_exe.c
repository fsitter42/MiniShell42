/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/18 11:49:21 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	f_child_process(t_data *data, t_cmd *cmd, int *prev_fd,
				int *pipe_fd);
static void	f_exec_builtin_child(t_cmd *cmd, t_data *data);
static void	f_parent_cleanup(t_cmd *cmd, int *prev_fd, int *pipe_fd);

int	f_exec_pipeline(t_data *data, t_cmd *cmd, int pipe_fd[2])
{
	int	prev_fd;

	prev_fd = -1;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
				return (f_pipe_error(data, cmd, &prev_fd));
		}
		data->pids->cpid[data->pids->i] = fork();
		if (data->pids->cpid[data->pids->i] == -1)
			return (f_fork_error(data, cmd, pipe_fd, &prev_fd));
		if (data->pids->cpid[data->pids->i] == 0)
			f_child_process(data, cmd, &prev_fd, pipe_fd);
		data->pids->i++;
		f_parent_cleanup(cmd, &prev_fd, pipe_fd);
		cmd = cmd->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (f_wait_all(data), data->last_exit_code);
}

static void	f_exec_builtin_child(t_cmd *cmd, t_data *data)
{
	int	status;

	f_exec_builtin(cmd, data);
	status = data->last_exit_code;
	sfbf_free_all(data);
	exit(status);
}

static void	f_parent_cleanup(t_cmd *cmd, int *prev_fd, int *pipe_fd)
{
	if (cmd->in_fd > 2)
	{
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd > 2)
	{
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

static void	f_child_process(t_data *data, t_cmd *cmd, int *prev_fd,
		int *pipe_fd)
{
	int	status;

	status = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	data->pids->i++;
	if (f_redir_wrapper(data, cmd) == -1)
	{
		status = data->last_exit_code;
		f_close_child(pipe_fd, prev_fd, cmd);
		sfbf_free_all(data);
		exit(status);
	}
	f_setup_pipe_fds(data, cmd, prev_fd, pipe_fd);
	f_setup_cmd_fds(data, cmd, pipe_fd, prev_fd);
	f_close_child(pipe_fd, prev_fd, cmd);
	if (cmd->is_builtin)
		f_exec_builtin_child(cmd, data);
	else
		f_exec_cmd(data, cmd, data->env->envp_updated);
}
