/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_ws_token_stuff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 17:35:11 by slambert          #+#    #+#             */
/*   Updated: 2026/04/05 17:40:19 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*create_split_token(char *word)
{
	t_token	*new;

	new = ft_calloc(sizeof(t_token), 1);
	if (!new)
		return (NULL);
	init_token(new);
	new->type = WORD;
	new->str = ft_strdup(word);
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static int	append_split_token(t_token **head, t_token **tail, char *word)
{
	t_token	*new;

	new = create_split_token(word);
	if (!new)
		return (ERROR_HARD);
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
	return (RET_OK);
}

int	create_and_fill_new_tokens(char **split_result, t_token *list)
{
	int		i;
	t_token	*orig;
	t_token	*new_head;
	t_token	*new_tail;
	int		ret;

	orig = list->next;
	new_head = NULL;
	new_tail = NULL;
	i = 1;
	while (split_result[i])
	{
		ret = append_split_token(&new_head, &new_tail, split_result[i]);
		if (ret != RET_OK)
			return (cleanup_token_list(new_head), ERROR_HARD);
		i++;
	}
	if (new_head)
	{
		list->next = new_head;
		new_tail->next = orig;
	}
	return (RET_OK);
}
