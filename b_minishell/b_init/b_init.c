/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:08:25 by slambert          #+#    #+#             */
/*   Updated: 2026/04/15 12:41:58 by fsitter          ###   ########.fr       */
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

t_pid *f_init_pid(void)
{
	t_pid *pids = ft_calloc(sizeof(t_pid), 1);
	if (!pids)
		return (NULL);
	pids->cc = 0;
	pids->
}


