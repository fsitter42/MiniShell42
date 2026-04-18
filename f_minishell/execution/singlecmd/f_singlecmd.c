/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_singlecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 01:47:36 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int		f_redir_setup(t_cmd *cmd, int saved_fds[2]);

void	f_exec_cmd(t_data *data, t_cmd *cmd, char **envp)
{
	int	status;

	cmd->path = f_path_handler(data, cmd->cmd, envp);
	if (!cmd->path)
	{
		status = data->last_exit_code;
		sfbf_free_all(data);
		exit(status);
	}
	execve(cmd->path, cmd->args, envp);
	f_print_error(cmd->cmd, "execve failed");
	sfbf_free_all(data);
	exit(88);
}

int	f_is_builtin(char *cmd)
{
	char	**bi;
	size_t	i;

	if (!cmd)
		return (0);
	i = 0;
	bi = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit", ".",
		NULL};
	while (bi[i])
	{
		if (ft_strncmp(cmd, bi[i], (ft_strlen(bi[i]) + 1)) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	f_redir_setup(t_cmd *cmd, int saved_fds[2])
{
	saved_fds[0] = dup(STDIN_FILENO);
	if (saved_fds[0] == -1)
		return (f_redir_error(saved_fds, -1, "dup"));
	saved_fds[1] = dup(STDOUT_FILENO);
	if (saved_fds[1] == -1)
		return (f_redir_error(saved_fds, -1, "dup"));
	if (cmd->in_fd != -1)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (f_redir_error(saved_fds, cmd->in_fd, "dup2"));
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd != -1)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (f_redir_error(saved_fds, cmd->out_fd, "dup2"));
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
	return (0);
}

void	f_redir_restore(int saved_fds[2], t_data *data)
{
	if (dup2(saved_fds[0], STDIN_FILENO) == -1)
	{
		f_print_error("dup2", strerror(errno));
		close(saved_fds[0]);
		close(saved_fds[1]);
		if (data->last_exit_code != 130)
			data->last_exit_code = 1;
		data->should_exit = 1;
		return ;
	}
	close(saved_fds[0]);
	if (dup2(saved_fds[1], STDOUT_FILENO) == -1)
	{
		f_print_error("dup2", strerror(errno));
		close(saved_fds[1]);
		if (data->last_exit_code != 130)
			data->last_exit_code = 1;
		data->should_exit = 1;
		return ;
	}
	close(saved_fds[1]);
}

int	f_exec_builtin(t_cmd *cmd, t_data *data)
{
	int	saved_fds[2];

	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (f_redir_wrapper(data, cmd) == -1)
		return (redir_return(data));
	if (f_redir_setup(cmd, saved_fds) == -1)
		return (data->should_exit = 1, data->last_exit_code = 1, -1);
	f_run_builtin(cmd, data);
	f_redir_restore(saved_fds, data);
	return (data->last_exit_code);
}
