/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 17:09:34 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TODO free(redir->file) eventuell wo anders hin JA DOUBLEFREE
void	remove_heredoc_files(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == HEREDOC && redir->file)
				unlink(redir->file);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}

int	eggsecute(t_data *data)
{
	int	status;

	if (!data->cmds->next && data->cmds->is_builtin)
		status = f_exec_builtin(data->cmds, data);
	else
	{
		f_pipeline_wrapper(data);
		status = data->last_exit_code;
	}
	remove_heredoc_files(data->cmds);
	return (status);
}

/* alt
TODO return return von f_exec_pipeline
TODO nur einmal ausführen, keine loop
int	eggsecute(t_data *data)
{
	t_cmd	*cmd_copy;

	cmd_copy = data->cmds;
	while (data->cmds)
	{
		// if (data->cmds->has_heredoc)
		//	do_heredoc_stuff(data->cmds);
		// eventuell hier checken ob is builtin
		data->cmds->is_builtin = f_is_builtin(data->cmds->cmd);
		if (!data->cmds->next && data->cmds->is_builtin)
			return (f_exec_builtin(data->cmds, data));
		f_exec_pipeline(data, data->cmds);
		// f_exec_pipeline(data, cmd_list);
		// delete .heredoc_dump
		data->cmds = data->cmds->next;
	}
	// TODO cleanup all cmds
	// TODO here we have to delete the temp heredoc files
	remove_heredoc_files(cmd_copy);
	return (data->last_exit_code);
} */