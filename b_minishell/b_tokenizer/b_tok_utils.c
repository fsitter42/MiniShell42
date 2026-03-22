/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tok_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:49:47 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 14:51:39 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->quote_status = DEFAULT_QUOTE;
	token->consume_status = UNCONSUMED;
	token->next = NULL;
	g_last_exit_code = 0;
}

t_token	*tokenlist_add(t_token *list_start, int type, char *str,
		int quote_status)
{
	t_token	*new_token;
	t_token	*current;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (free(str), NULL);
	current = list_start;
	while (current->next)
		current = current->next;
	current->next = new_token;
	init_token(new_token);
	new_token->type = type;
	new_token->str = str;
	new_token->quote_status = quote_status;
	return (new_token);
}

int	is_quote(char c)
{
	if (c == '\"' || c == '\'')
		return (1);
	return (0);
}

// returns 1 if not a word
int	not_a_word(char c1, char c2)
{
	if (c1 == '|')
		return (1);
	if (c1 == '<' && c2 == '<')
		return (1);
	if (c1 == '<')
		return (1);
	if (c1 == '>' && c2 == '>')
		return (1);
	if (c1 == '>')
		return (1);
	return (0);
}

int	is_part_of_word(char c, int *quote_status)
{
	if (*quote_status == DEFAULT_QUOTE && (c == '|' || c == '<' || c == '>'
			|| isspace(c) || c == '\0'))
		return (0);
	return (1);
}
