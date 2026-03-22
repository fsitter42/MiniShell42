/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 12:07:47 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void free_token (t_token *token)
{
	free (token->str);
	free (token);
}

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

void cleanup_redir_list(t_redir *redirs)
{
	t_redir	*temp;

	temp = NULL;
	while (redirs)
	{
		temp = redirs->next;
		free (redirs->file);
		free (redirs->delimiter);
		free (redirs);
		redirs = temp;
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
		//cmd_list->cmd = NULL;
		free(cmd_list->infile);
		free (cmd_list->outfile);
		while (cmd_list->args && cmd_list->args[i])
		{
			free(cmd_list->args[i]);
			i++;
		}
		free(cmd_list->args);
		free (cmd_list->delimiter);
		if (cmd_list->redirs)
			cleanup_redir_list(cmd_list->redirs);
		free (cmd_list);
		cmd_list = temp;
	}
}
void cleanup_t_data_list(t_data *data)
{
	cleanup_command_list(data->cmds);
	free(data);
}

void my_exit_function(char *error_msg)
{
	if (error_msg)
		printf("%s\n", error_msg);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	exit(0);
}
