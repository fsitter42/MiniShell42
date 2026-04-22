/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 00:09:05 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/22 18:05:00 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_open_infile(t_data *data, t_cmd *cmd, t_redir *redir)
{
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	cmd->in_fd = open(redir->file, O_RDONLY);
	if (cmd->in_fd == -1)
	{
		if (errno == ENOENT)
			f_print_error(redir->file, "No such file or directory");
		else if (errno == EACCES)
			f_print_error(redir->file, "Permission denied");
		else
			f_print_error(redir->file, strerror(errno));
		data->last_exit_code = 1;
		return (-1);
	}
	if (redir->type == HEREDOC)
	{
		unlink(redir->file);
		free(redir->file);
	}
	return (0);
}

int	f_open_outfile(t_data *data, t_cmd *cmd, char *file, int append)
{
	if (cmd->out_fd != -1)
		close(cmd->out_fd);
	if (append)
		cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
	{
		if (errno == EACCES)
			f_print_error(file, "Permission denied");
		else
			f_print_error(file, strerror(errno));
		data->last_exit_code = 1;
		return (-1);
	}
	return (0);
}
