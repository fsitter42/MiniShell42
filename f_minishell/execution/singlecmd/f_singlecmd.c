/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_singlecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a600 <a600@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/15 23:36:22 by a600             ###   ########.fr       */
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
	exit(126);
}

int	f_is_builtin(char *cmd)
{
	char	**bi;
	size_t	i;

	if (!cmd)
		return (0);
	i = 0;
	bi = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL};
	while (bi[i])
	{
		//SOLLTE PASSEN TODO wenn zb "e", "ec" etc. eingegeben wird, wird der input 
		//fälschlicherweise als builtin erkannt SOLLTE PASSEN
		if (ft_strncmp(cmd, bi[i], (ft_strlen(bi[i]) + 1)) == 0)
			return (1);
		i++;
	}
	return (0);
}

//TODO F blabalbla
//echo -ne "echo 'Hello World'\n" | env LD_PRELOAD="$HOME/42_minishell_tester/utils/libintercept/libintercept.so" valgrind --leak-check=full --show-leak-kinds=all --suppressions="$HOME/42_minishell_tester/utils/minishell.supp" --trace-children=yes --track-fds=all -s ./minishell

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
		return ;
	}
	close(saved_fds[0]);
	if (dup2(saved_fds[1], STDOUT_FILENO) == -1)
	{
		f_print_error("dup2", strerror(errno));
		close(saved_fds[1]);
		if (data->last_exit_code != 130)
			data->last_exit_code = 1;
		return ;
	}
	close(saved_fds[1]);
}

int	f_exec_builtin(t_cmd *cmd, t_data *data)
{
	int	saved_fds[2];

	if (f_redir_wrapper(data, cmd) == -1)
		return (redir_return(data));
		//return -1;
	if (f_redir_setup(cmd, saved_fds) == -1)
		return (data->last_exit_code = 1, -1);
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		data->last_exit_code = f_echo(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		data->last_exit_code = f_cd(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		data->last_exit_code = f_pwd(data);
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		data->last_exit_code = f_export(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		data->last_exit_code = f_unset(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		data->last_exit_code = f_env(data, cmd->args);
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		b_exit(data, cmd);
	f_redir_restore(saved_fds, data);
	return (data->last_exit_code);
}
