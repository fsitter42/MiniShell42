/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_main_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:12:31 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 12:15:49 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_token_list_empty(t_token *token_list)
{
	if (!token_list || !token_list->next)
		return (1);
	return (0);
}

int	find_delimiters(t_token *token_list)
{
	while (1)
	{
		while (token_list && token_list->type != HEREDOC)
			token_list = token_list->next;
		if (!token_list)
			return (RET_OK);
		if (token_list->next && token_list->next->type == WORD)
		{
			token_list->next->type = WORD_AFTER_HEREDOC;
			token_list = token_list->next;
		}
		else
			return (ERROR_SOFT);
	}
	return (RET_OK);
}

static int	is_whitespace(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\n')
		return (1);
	return (0);
}

int	line_is_empty(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!is_whitespace(line[i]))
			return (0);
	}
	return (1);
}
