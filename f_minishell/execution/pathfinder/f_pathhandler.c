/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pathhandler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:01:08 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/07 16:18:03 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*f_handle_direct_path(char *cmd, int *err, int *err2);
static void	f_print_126(int *err2);
static void	f_print_127(char *cmd, t_data *data);

char	*f_path_handler(t_data *data, char *cmd, char **envp)
{
	char	*path;
	int		err;
	int		err2;

	err = 0;
	err2 = 0;
	if (!cmd || !*cmd)
		return (NULL);
	path = f_handle_direct_path(cmd, &err, &err2);
	if (!path && err == 0)
		path = f_find_path(cmd, envp, &err, &err2);
	if (!path && err != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		if (err == 127)
			f_print_127(cmd, data);
		else if (err == 126)
			f_print_126(&err2);
		else if (err == 1)
			ft_putendl_fd(": memory allocation error", 2);
	}
	data->last_exit_code = err;
	return (path);
}

static void	f_print_127(char *cmd, t_data *data)
{
	if (cmd && ft_strchr(cmd, '/'))
		ft_putendl_fd(": No such file or directory", 2);
	else
	{
		if (f_path_set(data))
			ft_putendl_fd(": command not found", 2);
		else
			ft_putendl_fd(": No such file or directory", 2);
	}
}

static void	f_print_126(int *err2)
{
	if (*err2 != 0)
		ft_putendl_fd(": Is a directory", 2);
	else
		ft_putendl_fd(": Permission denied", 2);
}

static char	*f_handle_direct_path(char *cmd, int *err, int *err2)
{
	char	*res;

	if (cmd && ft_strchr(cmd, '/'))
	{
		f_validate_path(cmd, err, err2);
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
