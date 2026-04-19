/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_home_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:31:55 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 10:49:54 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// replaces a singular character with a string
// mem safe on error
static char	*replace_char_with_expandable(char *original, char char_to_expand,
		char *expandable)
{
	char	*temp;
	int		i_src;
	int		i_dst;
	int		size;

	i_src = 0;
	i_dst = 0;
	size = ft_strlen(original) - 1 + ft_strlen(expandable) + 1;
	temp = ft_calloc(size, sizeof(char));
	if (!temp)
		return (NULL);
	while (original[i_src])
	{
		if (original[i_src] == char_to_expand)
		{
			ft_strlcpy(&temp[i_dst], expandable, ft_strlen(expandable) + 1);
			i_dst += ft_strlen(expandable);
			i_src++;
			break ;
		}
		temp[i_dst++] = original[i_src++];
	}
	while (original[i_src])
		temp[i_dst++] = original[i_src++];
	return (temp);
}

static char	*extract_home_path_from_envp(char **envp, int *ret_status)
{
	char	*home;
	int		i;

	if (ret_status)
		*ret_status = RET_OK;
	home = NULL;
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "HOME=", 5) == 0)
		{
			home = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			if (!home && ret_status)
				*ret_status = ERROR_HARD;
			return (home);
		}
	}
	if (ret_status)
		*ret_status = ERROR_SOFT;
	return (NULL);
}

int	expand_home_dir(t_token *list_elem, char **envp)
{
	char	*home;
	char	*temp;
	int		ret;

	home = extract_home_path_from_envp(envp, &ret);
	if (!home)
		return (ret);
	temp = replace_char_with_expandable(list_elem->str, '~', home);
	if (!temp)
		return (free(home), ERROR_HARD);
	free(home);
	free(list_elem->str);
	list_elem->str = temp;
	return (RET_OK);
}

/*
 *	expands $? to the exit code number
 */
int	expand_dollar_question(char **out, int *i, t_data *data)
{
	char	*temp;

	temp = NULL;//ft_itoa(data->last_exit_code); // TODO wenn das failt exit code = 0 keine message
	if (!temp)
		return (ERROR_HARD);
	*out = append_str(*out, temp);
	free(temp);
	if (!*out)
		return (ERROR_HARD);
	*i += 1;
	return (RET_OK);
}
