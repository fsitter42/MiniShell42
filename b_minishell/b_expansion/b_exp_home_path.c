/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_home_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:31:55 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 17:28:45 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* we can't use getenv because the PATH variable could be changed and differ from
the PATH that is in envp (we call it with envp_updated)
TODO differ between var not found and ft_substr error -
do we maybe need a status var? */
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

int	expand_home_dir(t_token *list_elem, char **envp)
{
	char	*home;
	char	*temp;

	home = extract_home_path_from_envp(envp);
	// home = getenv("HOME");
	if (!home)
		return (ERROR_SOFT);
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

	temp = ft_itoa(data->last_exit_code);
	if (!temp)
		return (ERROR_HARD);
	*out = append_str(*out, temp);
	free(temp);
	if (!*out)
		return (ERROR_HARD);
	*i += 1;
	return (RET_OK);
}
