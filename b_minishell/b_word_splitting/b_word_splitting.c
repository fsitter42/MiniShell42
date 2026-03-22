/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_word_splitting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:22:19 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 14:59:57 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	normalizer(char *str, char *ifs, char ***split_result)
{
	char	*normalized;

	normalized = ft_strdup(str);
	if (!normalized)
		return (1);
	normalize_ifs_chars(normalized, ifs);
	*split_result = ft_split(normalized, ifs[0]);
	free(normalized);
	if (!split_result)
		return (1);
	return (0);
}

/*
 *  creates the new needed tokens
 *  TODO free the tokens that have been created (on failure)
 */
int	create_and_fill_new_tokens(char **split_result, t_token *list)
{
	int		i;
	t_token	*new;
	t_token	*orig;
	t_token	*tail;

	orig = list->next;
	tail = list;
	i = 1;
	while (split_result[i])
	{
		new = ft_calloc(sizeof(t_token), 1);
		if (!new)
			return (1);
		init_token(new);
		new->type = WORD;
		new->str = ft_strdup(split_result[i]);
		if (!new->str)
			return (free(new), 1);
		tail->next = new;
		tail = new;
		i++;
	}
	tail->next = orig;
	return (0);
}

/*
 *  executes the word splitting on a singular word. a word is split if it has
 *	IFS characters AND is not in double quotes
 */
int	split_single_word(t_token *list, char *ifs)
{
	char	**split_result;
	char	*new_first;

	if (list->str && ft_strchr_array(list->str, ifs))
	{
		if (normalizer(list->str, ifs, &split_result) == 1)
			return (1);
		if (split_result[0])
			new_first = ft_strdup(split_result[0]);
		else
			new_first = ft_strdup("");
		if (!new_first)
			return (free_str_array(split_result), 1);
		free(list->str);
		list->str = new_first;
		if (create_and_fill_new_tokens(split_result, list) == 1)
			return (free_str_array(split_result), 1);
		free_str_array(split_result);
	}
	return (0);
}

void	init_ifs_and_split(char **ifs, int *split)
{
	*split = 1;
	*ifs = getenv("IFS");
	if (!*ifs)
		*ifs = " \n\t";
	else if ((*ifs)[0] == '\0')
		*split = 0;
}

/*
 *   TODO give the command back to the user on empty input (now it is
 *   removed so no cmd is created, commandizer fails)
 */
int	word_split(t_token *list)
{
	char	*ifs;
	int		split;
	t_token	*prev;
	t_token	*next;

	init_ifs_and_split(&ifs, &split);
	prev = NULL;
	while (list)
	{
		next = list->next;
		if (list->type == WORD && list->str && list->str[0] == '\0')
		{
			remove_implicit_null_arg(&prev, next, &list);
			continue ;
		}
		if (list->type == WORD && split && list->quote_status == DEFAULT_QUOTE)
			if (split_single_word(list, ifs) == 1)
				return (1);
		prev = list;
		list = list->next;
	}
	return (0);
}
