/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/02/25 17:17:37 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void cleanup_token_list(t_token *token_list)
{
	t_token	*temp = NULL;
	
	while (token_list)
	{
		temp = token_list->next;
		free (token_list->str);
		free (token_list);
		token_list = temp;
	}
}

void cleanup_command_list(t_cmd *cmd_list)
{
	int i;
	t_cmd	*temp = NULL;
	
	while (cmd_list)
	{
		i = 0;
		temp = cmd_list->next;
		free(cmd_list->cmd);
		free(cmd_list->infile);
		free (cmd_list->outfile);
		while (cmd_list->args[i])
		{
			free(cmd_list->args[i]);
			i++;
		}
		free(cmd_list->args);
		free (cmd_list);
		cmd_list = temp;
	}
}

void my_exit_function()
{
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	exit(0);
}
