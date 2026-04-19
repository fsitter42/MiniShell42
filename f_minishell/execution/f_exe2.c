/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 10:57:05 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	f_open_redirections(t_data *data, t_cmd *cmd);
static int	f_open_all_heredocs(t_data *data, t_cmd *cmd);
static int	f_open_non_heredoc_redirs(t_data *data, t_cmd *cmd);

int	f_redir_wrapper(t_data *data, t_cmd *cmd)
{
	int	ret;

	data->cmds->in_fd = -1;
	data->cmds->out_fd = -1;
	ret = f_open_redirections(data, cmd);
	return (ret);
}

int	f_open_redirections(t_data *data, t_cmd *cmd)
{
	if (f_open_all_heredocs(data, cmd) == -1)
		return (-1);
	if (f_open_non_heredoc_redirs(data, cmd) == -1)
		return (-1);
	return (0);
}

static int	f_open_all_heredocs(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (b_handle_heredoc(data, cmd, redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

static int	f_open_non_heredoc_redirs(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (f_open_infile(data, cmd, redir->file) == -1)
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

int	redir_return(t_data *data)
{
	if (data->last_exit_code == 130)
		return (-1);
	else
		return (data->should_exit = 1, data->last_exit_code = 1, -1);
}
