/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/04/14 17:49:26 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	remove_heredoc_files(t_cmd *cmds)
{
	t_redir	*redir;

	redir = NULL;
	while (cmds)
	{
		if (cmds->redirs)
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
		//status = f_pipeline_wrapper(data);
		printf("hallo du beidl\n");
		status = 0;
	}
	remove_heredoc_files(data->cmds);
	return (status);
}
