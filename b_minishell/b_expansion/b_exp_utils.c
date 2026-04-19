/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:44 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 10:49:29 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_str(char *dst, char *src)
{
	char	*new_str;

	new_str = ft_strjoin(dst, src); // TODO wenn das failt exitcode = 0 und keine errormessage
	if (!new_str)
		return (free(dst), NULL);
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

static int	is_var_char(char c)
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
