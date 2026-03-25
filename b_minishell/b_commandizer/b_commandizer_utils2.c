/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_commandizer_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:54:58 by slambert          #+#    #+#             */
/*   Updated: 2026/03/25 16:02:01 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->is_builtin = -1;
	cmd->is_redir_only_cmd = 0;
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->in_fd = -1;
	cmd->outfile = NULL;
	cmd->out_fd = -1;
	cmd->next = NULL;
	cmd->append = FALSE;
	cmd->has_heredoc = FALSE;
	cmd->delimiter = NULL;
	cmd->redirs = NULL;
}

void	add_cmd_to_cmd_list(t_cmd **cmd_list, t_cmd *cmd)
{
	t_cmd	*current;

	if (!(*cmd_list))
	{
		*cmd_list = cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = cmd;
}
