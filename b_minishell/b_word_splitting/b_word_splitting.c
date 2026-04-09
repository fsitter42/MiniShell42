/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_word_splitting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:22:19 by slambert          #+#    #+#             */
/*   Updated: 2026/04/09 13:14:48 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 *  executes the word splitting on a singular word. a word is split if it has
 *	IFS characters AND is not in double quotes
 */
int	split_single_word(t_token *list, char *ifs)
{
	char	**split_result;
	int		ret;

	if (!list->str || !ft_strchr_array(list->str, ifs))
		return (RET_OK);
	ret = normalizer(list->str, ifs, &split_result);
	if (ret != RET_OK)
		return (ret);
	ret = replace_first_split_word(list, split_result);
	if (ret != RET_OK)
		return (free_str_array(split_result), ret);
	ret = create_and_fill_new_tokens(split_result, list);
	free_str_array(split_result);
	if (ret != RET_OK)
		return (ret);
	return (RET_OK);
}

static int	split_word_if_needed(t_token *list, char *ifs, int should_split)
{
	if (list->type == WORD && should_split && list->quote_status == DEFAULT_Q)
		return (split_single_word(list, ifs));
	return (RET_OK);
}

static int	process_word_split_loop(t_token *list, char *ifs, int should_split)
{
	t_token	*prev;
	t_token	*next;
	int		ret;

	prev = NULL;
	while (list)
	{
		next = list->next;
		if (is_implicit_null_word(list))
		{
			remove_implicit_null_arg(&prev, next, &list);
			continue ;
		}
		ret = split_word_if_needed(list, ifs, should_split);
		if (ret != RET_OK)
			return (ret);
		prev = list;
		list = list->next;
	}
	return (RET_OK);
}

//TODO da passt iwas nicht, wenn IFS gesetzt wird kann ich die var nicht
//mit echo anzeigen? 
static int	init_ifs_and_split(char **ifs, int *should_split, int *ifs_alloc,
		t_data *data)
{
	*should_split = 1;
	*ifs_alloc = 0;
	*ifs = f_get_env_val(data->env->envp_lst, "IFS", data);
	if (!*ifs && data->should_exit == 1)
		return (ERROR_HARD);
	else if (!*ifs)
		*ifs = " \n\t";
	else
	{
		*ifs_alloc = 1;
		if ((*ifs)[0] == '\0')
			*should_split = 0;
	}
	return (RET_OK);
}

/*
 *   TODO give the command back to the user on empty input (now it is
 *   removed so no cmd is created, commandizer fails)
 */
int	word_split(t_token *list, t_data *data)
{
	char	*ifs;
	int		should_split;
	int		ifs_alloc;
	int		ret;

	if (init_ifs_and_split(&ifs, &should_split, &ifs_alloc, data) == ERROR_HARD)
		return (ERROR_HARD);
	ret = process_word_split_loop(list, ifs, should_split);
	if (ret != RET_OK)
		return (cleanup_ifs_and_return(ifs, ifs_alloc, ret));
	if (ifs_alloc)
		free(ifs);
	return (RET_OK);
}
