/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_ws_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 17:38:20 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 15:14:17 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_implicit_null_word(t_token *list)
{
	int	ret;

	if (list->type != WORD || !list->str)
		return (FALSE);
	if (list->str[0] != '\0')
		return (FALSE);
	if (list->quote_status == DEFAULT_Q)
		ret = 1;
	else
		ret = 0;
	return (ret);
}

int	cleanup_ifs_and_return(char *ifs, int ifs_is_allocated, int ret)
{
	if (ifs_is_allocated)
		free(ifs);
	return (ret);
}

int	replace_first_split_word(t_token *list, char **split_result)
{
	char	*new_first;

	if (split_result[0])
		new_first = ft_strdup(split_result[0]);
	else
		new_first = ft_strdup("");
	if (!new_first)
		return (ERROR_HARD);
	free(list->str);
	list->str = new_first;
	return (RET_OK);
}

int	normalizer(char *str, char *ifs, char ***split_result)
{
	char	*normalized;

	normalized = ft_strdup(str);
	if (!normalized)
		return (ERROR_HARD);
	normalize_ifs_chars(normalized, ifs);
	*split_result = ft_split(normalized, ifs[0]);
	free(normalized);
	if (!*split_result)
		return (ERROR_HARD);
	return (RET_OK);
}
