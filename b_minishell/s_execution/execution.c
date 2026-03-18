/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/03/18 11:37:17 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 *  TODO
 *  can only be called AFTER execution
 */
void	remove_heredoc_file(void)
{
}

// TODO return return von f_exec_pipeline
// TODO nur einmal ausführen, keine loop
int	eggsecute(t_data *data)
{
	while (data->cmds)
	{
		//if (data->cmds->has_heredoc)
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
    return (data->last_exit_code);
}
