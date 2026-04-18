/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_ws_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:57:29 by slambert          #+#    #+#             */
/*   Updated: 2026/04/18 15:06:50 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr_array(char *s, char *arr)
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

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

// removes implcit null arguments (arguments that are empty - "")
void	remove_implicit_null_arg(t_token **prev, t_token *next, t_token **list)
{
	if (*prev)
		(*prev)->next = next;
	free_token(*list);
	*list = next;
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
void	normalize_ifs_chars(char *s, char *ifs)
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
