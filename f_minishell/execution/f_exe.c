/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/02 10:44:40 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	f_child_process(t_data *data, t_cmd *cmd, int prev_fd,
				int *pipe_fd);
static void	f_exec_builtin_child(t_cmd *cmd, t_data *data);
static void	f_wait_all(t_data *data);
static void	f_parent_cleanup(t_cmd *cmd, int *prev_fd, int *pipe_fd);

int	f_exec_pipeline(t_data *data, t_cmd *cmd, int pipe_fd[2])
{
	pid_t	pid;
	int		prev_fd;

	prev_fd = -1;
	while (cmd)
	{
		if (f_redir_wrapper(data, cmd) == -1)
			cmd->redir_failed = 1;
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
				exit(127);
		}
		pid = fork();
		if (pid == -1)
			f_close_pipe(cmd, pipe_fd, &prev_fd);
		if (pid == 0)
			f_child_process(data, cmd, prev_fd, pipe_fd);
		f_parent_cleanup(cmd, &prev_fd, pipe_fd);
		cmd = cmd->next;
	}
	return (f_wait_all(data), data->last_exit_code);
}

static void	f_child_process(t_data *data, t_cmd *cmd, int prev_fd, int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->redir_failed)
		exit(1);
	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (cmd->in_fd != -1)
		(dup2(cmd->in_fd, STDIN_FILENO), close(cmd->in_fd));
	if (cmd->out_fd != -1)
		(dup2(cmd->out_fd, STDOUT_FILENO), close(cmd->out_fd));
	f_close_child(pipe_fd, prev_fd, cmd);
	if (cmd->is_builtin)
		f_exec_builtin_child(cmd, data);
	else
		f_exec_cmd(data, cmd, data->env->envp_updated);
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
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

static void	f_wait_all(t_data *data)
{
	int		status;
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while ((waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status))
			data->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			data->last_exit_code = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_printf("Quit (core dumped)\n");
		}
	}
	setup_signals();
}
