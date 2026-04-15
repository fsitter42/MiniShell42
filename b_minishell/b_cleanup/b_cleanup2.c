/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cleanup2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/04/15 16:48:08 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cleanup_split_result(char **strs, int start)
{
	if (!strs)
		return ;
	while (strs[start])
	{
		free(strs[start]);
		start++;
	}
	free(strs);
}

void	sfbf_free_all(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->env->envp_lst)
		f_free_env_list(data->env->envp_lst); // vll nmoch freen zusätzliuch
	if (data->env->envp_updated)
		f_free_envp(data->env->envp_updated);
	free(data->env);
	i = 0;
	while (data->strs && data->strs[i])
	{
		free(data->strs[i]);
		i++;
	}
	if (data->strs)
		free(data->strs);
	cleanup_command_list(data->cmds);
	// TODO remove das before abgabe, das is fürs get_next_line funchecken
	// get_next_line(-1);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	free(data);
}

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v'
		|| c == '\f')
		return (1);
	return (0);
}

void	ft_putendl_fd_no_nl(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}