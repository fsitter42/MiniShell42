/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/18 00:19:47 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	f_child_process(t_data *data, t_cmd *cmd, int prev_fd,
				int *pipe_fd);
static void	f_exec_builtin_child(t_cmd *cmd, t_data *data);
static void	f_wait_all(t_data *data);
static void	f_parent_cleanup(t_cmd *cmd, int *prev_fd, int *pipe_fd);

int	f_exec_pipeline(t_data *data, t_cmd *cmds)
{
	t_cmd	*cmd;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;

	// if (!cmds->next && cmds->is_builtin != -1)
	// 	return (f_exec_builtin(cmds, data), 0);
	cmd = cmds;
	prev_fd = -1;
	while (cmd)
	{
		if (f_open_redirections(data, cmd) == -1)
			return (f_wait_all(data), 1);
		if (cmd->next)
			pipe(pipe_fd); //if -1 here
		pid = fork(); //if -1
		if (pid == 0)
			f_child_process(data, cmd, prev_fd, pipe_fd);
		f_parent_cleanup(cmd, &prev_fd, pipe_fd);
		cmd = cmd->next;
	}
	return (f_wait_all(data), 0);
}

static void	f_child_process(t_data *data, t_cmd *cmd, int prev_fd, int *pipe_fd)
{
	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (cmd->in_fd != -1)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd != -1)
		dup2(cmd->out_fd, STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (cmd->redir_failed) //here löschen
		exit(1);
	if (cmd->is_builtin)
		f_exec_builtin_child(cmd, data);
	else
		f_exec_cmd(data, cmd, data->env->envp_updated);
}

static void	f_exec_builtin_child(t_cmd *cmd, t_data *data)
{
	f_exec_builtin(cmd, data);
	exit(data->last_exit_code);
}

static void	f_parent_cleanup(t_cmd *cmd, int *prev_fd, int *pipe_fd)
{
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
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			data->last_exit_code = WEXITSTATUS(status);
	}
}

/*no global last exit code how to handel ??? frido */