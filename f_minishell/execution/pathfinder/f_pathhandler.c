/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pathhandler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:01:08 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/17 23:54:52 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*f_handle_direct_path(char *cmd, int *err);

char	*f_path_handler(t_data *data, char *cmd, char **envp)
{
	char	*path;
	int		err;

	err = 0;
	if (!cmd || !*cmd)
		return (NULL);
	path = f_handle_direct_path(cmd, &err);
	if (!path && err == 0)
		path = f_find_path(cmd, envp, &err);
	if (!path && err != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		if (err == 127)
			ft_putendl_fd(": command not found", 2);
		else if (err == 126)
			ft_putendl_fd(": Permission denied or is a directory", 2);
		else if (err == 1)
			ft_putendl_fd(": memory allocation error", 2);
	}
	data->last_exit_code = err;
	return (path);
}

static char	*f_handle_direct_path(char *cmd, int *err)
{
	char	*res;

	if (cmd && ft_strchr(cmd, '/'))
	{
		f_validate_path(cmd, err);
		if (*err != 0)
			return (NULL);
		res = ft_strdup(cmd);
		if (!res)
			*err = 1;
		return (res);
	}
	return (NULL);
}

void	f_print_error(char *context, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

// char	*f_path_handler(char *cmd, char **envp)
// {
// 	char	*path;
// 	int		err;

// 	err = 0;
// 	if (!cmd || !*cmd)
// 		return (NULL);
// 	path = f_handle_direct_path(cmd, &err);
// 	if (!path && err == 0)
// 		path = f_find_path(cmd, envp, &err);
// 	if (!path && err != 0)
// 	{
// 		if (err == 127)
// 			f_print_error(cmd, "command not found");
// 		else if (err == 126)
// 			f_print_error(cmd, "Permission denied or is a directory");
// 		else if (err == 1)
// 			f_print_error(cmd, "memory allocation error");
// 	}
// 	g_last_exit_code = err;
// 	return (path);
// }