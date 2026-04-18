/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_commandizer_utils1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:44:31 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 00:52:17 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_token *token_list)
{
	int	pipe_count;

	pipe_count = 0;
	while (token_list)
	{
		if (token_list->type == PIPE)
			pipe_count++;
		token_list = token_list->next;
	}
	return (pipe_count);
}

void	shift_tl_to_next_pipe(t_token **token_list)
{
	while (*token_list && (*token_list)->type != PIPE)
		*token_list = (*token_list)->next;
	if (!*token_list)
		return ;
	*token_list = (*token_list)->next;
}

int	count_size_for_args_array(t_token *token_list)
{
	int	size;

	size = 0;
	while (token_list && token_list->type != PIPE)
	{
		if (is_token_type_redirection(token_list))
		{
			if (!token_list->next)
				return (-1);
			token_list = token_list->next;
		}
		else
			size++;
		token_list = token_list->next;
	}
	return (size);
}

int	init_args_array(t_cmd *cmd, int size)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(size + 1, sizeof(char *));
	if (!(cmd->args))
		return (ERROR_HARD);
	return (RET_OK);
}

void	shift_and_consume_tl(t_token **token, int x)
{
	int	i;

	i = -1;
	while (++i < x)
	{
		(*token)->consume_status = CONSUMED;
		(*token) = (*token)->next;
	}
}
