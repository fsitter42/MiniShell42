/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_dot.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 23:40:13 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 01:41:00 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	f_print_dot_error(t_data *data, char **args)
{
	char	buf[512];
	int		len;

	len = 0;
	if (!args || !args[1])
	{
		len += ft_strlcpy(buf + len,
				"minishell: .: filename argument required\n", sizeof(buf)
				- len);
		len += ft_strlcpy(buf + len, ".: usage: . filename [arguments]\n",
				sizeof(buf) - len);
	}
	else
	{
		len += ft_strlcpy(buf + len, "minishell: .: ", sizeof(buf) - len);
		len += ft_strlcpy(buf + len, args[1], sizeof(buf) - len);
		len += ft_strlcpy(buf + len, ": is a directory\n", sizeof(buf) - len);
	}
	write(2, buf, len);
	data->last_exit_code = 2;
}

void	f_run_builtin(t_cmd *cmd, t_data *data)
{
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
	else if (ft_strncmp(cmd->cmd, ".", 2) == 0)
		f_print_dot_error(data, cmd->args);
}
