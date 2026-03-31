/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cleanup2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 12:07:37 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



void	cleanup_split_result(char **strs, int start)
{
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
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	free(data);
}
