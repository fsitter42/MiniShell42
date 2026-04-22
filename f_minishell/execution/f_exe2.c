/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/22 20:39:57 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	f_open_redirs(t_data *data, t_cmd *cmd);

int	f_redir_wrapper(t_data *data, t_cmd *cmd)
{
	int	ret;

	data->cmds->in_fd = -1;
	data->cmds->out_fd = -1;
	ret = f_open_redirs(data, cmd);
	return (ret);
}

int	f_open_all_heredocs_for_cmd(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (b_handle_heredoc(data, redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

static int	f_open_redirs(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			if (f_open_infile(data, cmd, redir) == -1)
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
