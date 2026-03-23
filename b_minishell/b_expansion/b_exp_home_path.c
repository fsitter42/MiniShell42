/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_home_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:31:55 by slambert          #+#    #+#             */
/*   Updated: 2026/03/23 12:53:48 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// getenv would be allowed
char	*extract_home_path_from_envp(char **envp)
{
	char	*home;
	int		i;

	home = NULL;
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "HOME=", 5) == 0)
			return (ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5));
	}
	return (NULL);
}

// replaces a singular character with a string
// mem safe on error
char	*replace_char_with_expandable(char *original, char char_to_expand,
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

/*
 *  memory safe on error
 *	TODO change to getenv
 */
int	expand_home_dir(t_token *list_elem, char **envp)
{
	char	*home;
	char	*temp;

	home = NULL;
	home = extract_home_path_from_envp(envp);
	if (!home)
		return (1);
	temp = replace_char_with_expandable(list_elem->str, '~', home);
	if (!temp)
		return (free(home), 1);
	free(home);
	free(list_elem->str);
	list_elem->str = temp;
	return (0);
}

/*
 *	expands $? to the exit code number
 */
int	expand_dollar_question(char **out, int *i, t_data *data)
{
	char	*temp;

	temp = ft_itoa(data->last_exit_code);
	if (!temp)
		return (1);
	*out = append_str(*out, temp);
	free(temp);
	if (!*out)
		return (1);
	*i += 1;
	return (0);
}
