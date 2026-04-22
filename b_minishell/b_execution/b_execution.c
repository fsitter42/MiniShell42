/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/04/22 19:03:16 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_pid	*f_init_pid(t_data *data);
static int		f_command_count(t_data *data);
t_pid			*f_free_pids(t_data *data);

int	open_all_heredocs(t_data *data)
{
	t_cmd	*cur_cmd;

	cur_cmd = data->cmds;
	while (cur_cmd)
	{
		if (f_open_all_heredocs_for_cmd(data, cur_cmd) == -1)
			return (-1);
		cur_cmd = cur_cmd->next;
	}
	return (0);
}

int	eggsecute(t_data *data)
{
	int	status;

	if (open_all_heredocs(data) == -1)
	{
		if (data->last_exit_code != 130)
			data->last_exit_code = 1;
		return (data->last_exit_code);
	}
	if (!data->cmds->next && data->cmds->is_builtin)
		status = f_exec_builtin(data->cmds, data);
	else
	{
		data->pids = f_init_pid(data);
		if (!data->pids)
		{
			status = 1;
			data->should_exit = 1;
		}
		else
			status = f_pipeline_wrapper(data);
	}
	if (data->pids)
		data->pids = f_free_pids(data);
	data->last_exit_code = status;
	return (status);
}

static t_pid	*f_init_pid(t_data *data)
{
	t_pid	*pids;

	pids = ft_calloc(sizeof(t_pid), 1);
	if (!pids)
		return (NULL);
	pids->i = 0;
	pids->cc = 0;
	pids->cpid = NULL;
	pids->cc = f_command_count(data);
	if (pids->cc > 0)
	{
		pids->cpid = ft_calloc(sizeof(pid_t), pids->cc);
		if (!pids->cpid)
		{
			free(pids);
			return (NULL);
		}
	}
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

t_pid	*f_free_pids(t_data *data)
{
	if (!data)
		return (NULL);
	if (data->pids && data->pids->cpid)
		free(data->pids->cpid);
	if (data->pids)
		free(data->pids);
	return (NULL);
}

/* void	remove_heredoc_files(t_cmd *cmds)
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
} */