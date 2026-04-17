/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cleanup1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/04/17 22:04:11 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_token(t_token *token)
{
	free(token->str);
	free(token);
}

void	cleanup_token_list(t_token *token_list)
{
	t_token	*temp;

	temp = NULL;
	while (token_list)
	{
		temp = token_list->next;
		free(token_list->str);
		free(token_list);
		token_list = temp;
	}
}

void	cleanup_redir_list(t_redir *redirs)
{
	t_redir	*temp;

	temp = NULL;
	while (redirs)
	{
		temp = redirs->next;
		if (redirs->type == HEREDOC && redirs->file)
			unlink(redirs->file);
		free(redirs->file);
		free(redirs->delimiter);
		free(redirs);
		redirs = temp;
	}
}

void	cleanup_command_list2(t_cmd *cmd_list)
{
	int		i;
	t_cmd	*temp;

	temp = NULL;
	while (cmd_list)
	{
		i = 0;
		temp = cmd_list->next;
		free(cmd_list->cmd);
		free(cmd_list->path);
		while (cmd_list->args && cmd_list->args[i])
		{
			free(cmd_list->args[i]);
			i++;
		}
		free(cmd_list->args);
		if (cmd_list->redirs)
			cleanup_redir_list(cmd_list->redirs);
		free(cmd_list);
		cmd_list = temp;
	}
}
// TODOF das macht leak weg??? von dem spazialtester 
void	cleanup_command_list(t_cmd *cmd_list)
{
    int     i;
    t_cmd   *temp;

    temp = NULL;
    while (cmd_list)
    {
        i = 0;
        temp = cmd_list->next;
        free(cmd_list->cmd);
        free(cmd_list->path);
        while (cmd_list->args && cmd_list->args[i])
        {
            free(cmd_list->args[i]);
            i++;
        }
        free(cmd_list->args);
        if (cmd_list->redirs)
            cleanup_redir_list(cmd_list->redirs);
        if (cmd_list->in_fd != -1)
            close(cmd_list->in_fd);
        if (cmd_list->out_fd != -1)
            close(cmd_list->out_fd);
        free(cmd_list);
        cmd_list = temp;
    }
}


void	cleanup_t_data_list(t_data *data)
{
	cleanup_command_list(data->cmds);
	free(data);
}
