/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/19 14:15:24 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	f_open_infile(t_data *data, t_cmd *cmd, char *file);
static int	f_open_outfile(t_data *data, t_cmd *cmd, char *file, int append);

int	f_open_redirections(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	cmd->in_fd = -1;
	cmd->out_fd = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (f_open_infile(data, cmd, redir->file) == -1)
				return (-1);
		}
		else if (redir->type == HEREDOC)
		{
			if (b_handle_heredoc(data, cmd, redir) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (f_open_outfile(data, cmd, redir->file,
					redir->type == REDIR_APPEND) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

static int	f_open_infile(t_data *data, t_cmd *cmd, char *file)
{
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	cmd->in_fd = open(file, O_RDONLY);
	if (cmd->in_fd == -1)
	{
		if (errno == ENOENT)
			f_print_error(file, "No such file or directory");
		else if (errno == EACCES)
			f_print_error(file, "Permission denied");
		else
			f_print_error(file, strerror(errno));
		data->last_exit_code = 1;
		return (-1);
	}
	return (0);
}

static int	f_open_outfile(t_data *data, t_cmd *cmd, char *file, int append)
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

/*n*/