/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_commandizer_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:54:58 by slambert          #+#    #+#             */
/*   Updated: 2026/03/30 17:27:58 by slambert         ###   ########.fr       */
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
	//cmd->infile = NULL;
	cmd->in_fd = -1;
	//cmd->outfile = NULL;
	cmd->out_fd = -1;
	cmd->next = NULL;
	cmd->append = FALSE;
	//cmd->has_heredoc = FALSE;
	cmd->delimiter = NULL;
	cmd->redirs = NULL;
	cmd->is_first = 0;
}

void	add_cmd_to_cmd_list(t_cmd **cmd_list, t_cmd *cmd)
{
	t_cmd		*current;

	if (!(*cmd_list))
	{
		*cmd_list = cmd;
		cmd->is_first = 1;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = cmd;
}
