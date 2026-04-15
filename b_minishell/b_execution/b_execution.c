/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/04/15 13:03:17 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_pid	*f_init_pid(t_data *data);
static int		f_command_count(t_data *data);

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
		data->pids = f_init_pid(data);
		if (!data->pids)
			status = 1;
		else
			// status = f_pipeline_wrapper(data);
		printf("%i\n", data->pids->i);
		status = 0;
	}
	if (data->pids)
		f_free_pids(data);
	remove_heredoc_files(data->cmds);
	return (status);
}

static t_pid	*f_init_pid(t_data *data)
{
	t_pid	*pids;

	pids = ft_calloc(sizeof(t_pid), 1);
	if (!pids)
		return (NULL);
	pids->cc = 0;
	pids->cc = f_command_count(data);
	pids->i = 0;
	return (pids);
}

static int	f_command_count(t_data *data)
{
	int		cc;
	t_cmd	*current;

	cc = 0;
	current = data->cmds;
	while (current)
	{
		cc++;
		current = current->next;
	}
	return (cc);
}

t_pid *f_free_pids(data)
{
	//built this
	return (NULL);
}
