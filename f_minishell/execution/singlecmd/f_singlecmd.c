/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_singlecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/19 14:05:43 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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
	// bi = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
	// 	NULL};
	bi = (char *[]){"cd", "pwd", "export", "unset", "env", "exit", NULL};
	while (bi[i])
	{
		if (ft_strncmp(cmd, bi[i], len) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	f_exec_builtin(t_cmd *cmd, t_data *data)
{
	// ich brauche hier auch redirections!!!!!!!
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		printf("echo\n"); // f_echo();
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		printf("cd\n"); // f_cd();
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		printf("pwd\n"); // f_pwd();
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		data->last_exit_code = f_export(data, data->cmds->args); // f_export();
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		printf("unset\n"); // f_unset();
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		printf("env\n"); // f_env(data, cmd->args) ; // f_env();
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		printf("exit\n");
	; // f_exit();
	return (data->last_exit_code);
}

// f_exec_builtin(cmd);

/*
	ich bin hier::::: diese drei funktionen in einen neuen ordner singlecomands geben
	builtinchecker vorschalten
	< > >> einauen

	neuer ordner pipes
	loop für multiple commands
	zusammenfügen fertig :D

*/

/*n*/