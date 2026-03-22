/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:44 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 14:41:26 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_str(char *dst, char *src)
{
	char	*new_str;

	new_str = ft_strjoin(dst, src);
	free(dst);
	return (new_str);
}

char	*append_char(char *dst, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_str(dst, tmp));
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	get_var_name_len(char *word, int i)
{
	int	len;

	len = 0;
	while (word[i + 1 + len] && is_var_char(word[i + 1 + len]))
		len++;
	return (len);
}

void	add_to_list(t_token *list, t_token *new)
{
	while (list->next)
		list = list->next;
	list->next = new;
}
