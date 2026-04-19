/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pathhandler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:01:08 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 15:13:47 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*f_handle_direct_path(char *cmd, int *err, int *err2);

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
		f_print_path_error(cmd, err, err2, data);
	data->last_exit_code = err;
	return (path);
}

static char	*f_handle_direct_path(char *cmd, int *err, int *err2)
{
	char	*res;

	if (cmd && ft_strncmp(cmd, "..", 3) == 0)
		return (*err = 127, NULL);
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
	char	buf[512];
	int		len;

	len = 0;
	len += ft_strlcpy(buf + len, "minishell: ", sizeof(buf) - len);
	len += ft_strlcpy(buf + len, context, sizeof(buf) - len);
	len += ft_strlcpy(buf + len, ": ", sizeof(buf) - len);
	len += ft_strlcpy(buf + len, msg, sizeof(buf) - len - 1);
	buf[len++] = '\n';
	write(2, buf, len);
}
