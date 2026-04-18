/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_unsetpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:41:07 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/18 11:52:56 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_path_set(t_data *data)
{
	t_envl	*current;

	current = data->env->envp_lst;
	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 5) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	f_print_127(char *cmd, t_data *data, char *buf, int *len)
{
	if (cmd && ft_strchr(cmd, '/'))
		*len += ft_strlcpy(buf + *len, ": No such file or directory\n", 512
				- *len);
	else if (f_path_set(data))
		*len += ft_strlcpy(buf + *len, ": command not found\n", 512 - *len);
	else
		*len += ft_strlcpy(buf + *len, ": No such file or directory\n", 512
				- *len);
}

void	f_print_126(int err2, char *buf, int *len)
{
	if (err2 != 0)
		*len += ft_strlcpy(buf + *len, ": Is a directory\n", 512 - *len);
	else
		*len += ft_strlcpy(buf + *len, ": Permission denied\n", 512 - *len);
}

void	f_print_path_error(char *cmd, int err, int err2, t_data *data)
{
	char	buf[512];
	int		len;

	len = 0;
	len += ft_strlcpy(buf + len, "minishell: ", sizeof(buf) - len);
	len += ft_strlcpy(buf + len, cmd, sizeof(buf) - len);
	if (err == 127)
		f_print_127(cmd, data, buf, &len);
	else if (err == 126)
		f_print_126(err2, buf, &len);
	else if (err == 1)
		len += ft_strlcpy(buf + len, ": memory allocation error\n", sizeof(buf)
				- len);
	write(2, buf, len);
}
