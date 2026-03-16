/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pathfinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:59:42 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:04:00 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*f_get_path_var(char **envp);
static char	*f_join_cmd_to_path(char *dir, char *cmd);

static char	*f_get_path_var(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*f_join_cmd_to_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	if (!full)
		return (free(tmp), NULL);
	free(tmp);
	return (full);
}

void	f_validate_path(char *path, int *err)
{
	struct stat	s;

	if (access(path, F_OK) != 0)
	{
		if (*err != 126)
			*err = 127;
		return ;
	}
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		*err = 126;
	else if (access(path, X_OK) != 0)
		*err = 126;
	else
		*err = 0;
}

char	*f_find_path(char *cmd, char **envp, int *err)
{
	char	**dirs;
	char	*full;
	int		i;

	full = f_get_path_var(envp);
	if (!full)
		return (*err = 127, NULL);
	dirs = ft_split(full, ':');
	if (!dirs)
		return (*err = 1, NULL);
	*err = 127;
	i = -1;
	while (dirs[++i])
	{
		full = f_join_cmd_to_path(dirs[i], cmd);
		if (!full)
			return (*err = 1, f_free_envp(dirs), NULL);
		f_validate_path(full, err);
		if (*err == 0)
			return (f_free_envp(dirs), full);
		free(full);
	}
	return (f_free_envp(dirs), NULL);
}
