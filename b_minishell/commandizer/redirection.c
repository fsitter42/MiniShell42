/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:51:27 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 13:01:45 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_token_type_redirection(t_token *token)
{
	int	x;

	x = token->type;
	if (x == HEREDOC || x == REDIR_IN || x == REDIR_APPEND || x == REDIR_OUT)
		return (1);
	return (0);
}

// returns 0 is the redirection target is not valid
int	is_valid_redirection_target(t_token *redir_token)
{
	if (!redir_token || !redir_token->next)
		return (0);
	if (redir_token->next->type != WORD
		&& redir_token->next->type != WORD_AFTER_HEREDOC)
		return (0);
	if (!redir_token->next->str)
		return (0);
	return (1);
}

void	add_redir_to_redir_list(t_redir *new_redir, t_cmd *cmd)
{
	t_redir	*cur;

	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		cur = cmd->redirs;
		while (cur->next)
			cur = cur->next;
		cur->next = new_redir;
	}
}

int	next_redir_id(void)
{
	static int	id;

	id++;
	if (id <= 0)
		id = 1;
	return (id);
}

int	add_r_t_c(t_cmd *cmd, int type, char *str, char *delimiter)
{
	t_redir	*new_redir;

	new_redir = ft_calloc(sizeof(t_redir), 1);
	if (!new_redir)
		return (1);
	if (delimiter)
	{
		new_redir->delimiter = ft_strdup(delimiter);
		if (!new_redir->delimiter)
			return (free(new_redir), 1);
	}
	if (str)
	{
		new_redir->file = ft_strdup(str);
		if (!new_redir->file)
			return (free(new_redir->delimiter), free(new_redir), 1);
	}
	new_redir->type = type;
	new_redir->id = next_redir_id();
	add_redir_to_redir_list(new_redir, cmd);
	return (0);
}
