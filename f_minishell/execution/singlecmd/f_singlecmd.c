/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_singlecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/21 12:42:47 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	f_redir_restore(int saved_fds[2]);
static void	f_redir_restore(int saved_fds[2]);

void	f_exec_cmd(t_data *data, t_cmd *cmd, char **envp)
{
	cmd->path = f_path_handler(data, cmd->cmd, envp);
	// here frido wo gehört das hin
	if (!cmd->path)
		exit(data->last_exit_code);
	execve(cmd->path, cmd->args, envp);
	f_print_error(cmd->cmd, "execve failed");
	exit(1);
}

int	f_is_builtin(char *cmd)
{
	char	**bi;
	size_t	i;
	size_t	len;

	if (!cmd)
		return (0);
	len = ft_strlen(cmd);
	i = 0;
	bi = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL};
	while (bi[i])
	{
		if (ft_strncmp(cmd, bi[i], len) == 0)
			return (1);
		i++;
	}
	return (0);
}

// int	f_exec_builtin(t_cmd *cmd, t_data *data)
// {
// 	// ich brauche hier auch redirections!!!!!!!
// 	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
// 		printf("echo\n"); // f_echo();
// 	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
// 		printf("cd\n"); // f_cd();
// 	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
// 		printf("pwd\n"); // f_pwd();
// 	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
// 		data->last_exit_code = f_export(data, data->cmds->args); // f_export();
// 	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
// 		printf("unset\n"); // f_unset();
// 	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
// 		data->last_exit_code = f_env(data, cmd->args);
// 	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
// 		printf("exit\n");
// 	; // f_exit();
// 	return (data->last_exit_code);
// }

int	f_redir_setup(t_cmd *cmd, int saved_fds[2])
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (cmd->in_fd != -1)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (-1);
		close(cmd->in_fd);
	}
	if (cmd->out_fd != -1)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (-1);
		close(cmd->out_fd);
	}
	return (0);
	//add errors data->lastexitcode
}

void	f_redir_restore(int saved_fds[2])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
	//add errors data->lastexitcode
}

int	f_exec_builtin(t_cmd *cmd, t_data *data)
{
	int	saved_fds[2];

	if (f_redir_wrapper(data, cmd) == -1)
		return (1);
	if (f_redir_setup(cmd, saved_fds) == -1)
		return (1);
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		data->last_exit_code = f_echo(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		data->last_exit_code = f_cd(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		printf("pwd\n"); // data->last_exit_code = f_pwd();
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		data->last_exit_code = f_export(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		data->last_exit_code = f_unset(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		data->last_exit_code = f_env(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		printf("exit\n"); // f_exit(data, cmd->args);
	f_redir_restore(saved_fds);
	return (data->last_exit_code);
}

/*n*/