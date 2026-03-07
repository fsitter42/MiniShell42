/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:22:19 by slambert          #+#    #+#             */
/*   Updated: 2026/03/06 13:38:28 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*   TODO implement a "protection mask" during expansion in order to  keep the
*   information whether a character should actually be considered for word
*   splitting (that depends on if it is in quotes or not)
*/
char *ft_strchr_array(char *s, char *arr)
{
	int		i;
	int		j;
	char	c;

	i = ft_strlen(s);
	while (i >= 0)
	{
		j = -1;
		while (arr[++j])
		{
			c = arr[j];
			if ((unsigned char)s[i] == (unsigned char)c)
				return ((char *)&s[i]);
		}
		i--;
	}
	return (NULL);
}

static void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

static int	is_ifs_char(char c, char *ifs)
{
	int	i;

	i = 0;
	while (ifs[i])
	{
		if (ifs[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/*
*  bc ft_split takes on character as a delimiter we somehow have to
*  "fake" IFS so that each occurence is only 1 char
*/ 
static void	normalize_ifs_chars(char *s, char *ifs)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (is_ifs_char(s[i], ifs))
			s[i] = ifs[0];
		i++;
	}
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
		if (!new->str) // TODO better error mgmt
			return (free(new), 1);
		tail->next = new;
		tail = new;
		i++;
	}
	tail->next = orig;
	return (0);
}

/*
 *  executes the word splitting in a singular word. a word is split if it has IFS characters
 *  AND is not in double quotes
 */
int	split_single_word(t_token *list, char *ifs)
{
	char	**split_result;
	char	*normalized;
	char	*new_first;
	
	if (list->str && ft_strchr_array(list->str, ifs))
	{
		normalized = ft_strdup(list->str);
		if (!normalized)
			return (1);
		normalize_ifs_chars(normalized, ifs);
		split_result = ft_split(normalized, ifs[0]);
		free(normalized);
		if (!split_result)
			return (1);
		if (split_result[0])
			new_first = ft_strdup(split_result[0]);
		else
			new_first = ft_strdup("");
		if (!new_first)
			return (free_str_array(split_result), 1);
		free(list->str);
		list->str = new_first;
		// restliche elemente in split_result (also ab 1) werden neue token
		if (create_and_fill_new_tokens(split_result, list) == 1)
			return (free_str_array(split_result), 1);
		free_str_array(split_result);
	}
	return (0);
}

/* 
*   TODO give the command back to the user on empty input (now it is 
*   removed so no cmd is created, commandizer fails)
 */
int word_split(t_token *list)
{
	char	*ifs;
	int		split;
	t_token	*prev;
	t_token	*next;

	split = 1;
	ifs = getenv("IFS");
	if (!ifs)
		ifs = " \n\t";
	else if (ifs[0] == '\0')
		split = 0;
	prev = NULL;
	while (list)
	{
		next = list->next;
		if (list->type == WORD && list->str && list->str[0] == '\0')
		{
			//removes implcit null arguments (arguments that are empty - "")
			if (prev)
				prev->next = next;
			free_token(list);
			list = next;
			continue ;
		}
		if (list->type == WORD && split)
		{
			if (split_single_word(list, ifs) == 1)
				return (1);
		}
		prev = list;
		list = list->next;
	}
	return (0);
}
