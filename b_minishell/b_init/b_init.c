/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:08:25 by slambert          #+#    #+#             */
/*   Updated: 2026/04/09 16:44:01 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_data	*sfbf_init_all(char **envp)
{
	t_data	*data;
	char	**args;

	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
		return (NULL);
	data->env = f_init_envp(envp);
	if (!data->env)
		return (free(data), NULL);
	args = (char *[]){"export", "OLDPWD", NULL};
	if (f_export(data, args) == EXIT_FAILURE)
	{
		f_destroy_envp(data->env);
		free(data);
		return (NULL);
	}
	data->strs = NULL;
	data->last_exit_code = 0;
	data->should_exit = 0;
	data->e_has_been_set = 0;
	data->ret_from_hsl = 0;
	return (data);
}
