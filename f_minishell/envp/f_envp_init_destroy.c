/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_envp_init_destroy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 23:58:21 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/30 11:48:34 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_envp	*f_init_envp(char **envp)
{
	t_envp	*p;

	p = ft_calloc(sizeof(t_envp), 1);
	if (!p)
		return (NULL);
	p->envp_ori = envp;
	p->envp_lst = f_copy_env(envp);
	if (!p->envp_lst)
		return (free(p), NULL);
	if (f_update_envp(p, NULL))
		return (f_destroy_envp(p));
	return (p);
}

t_envp	*f_destroy_envp(t_envp *p)
{
	if (!p)
		return (NULL);
	p->envp_ori = NULL;
	if (p->envp_lst)
	{
		f_free_env_list(p->envp_lst);
		p->envp_lst = NULL;
	}
	if (p->envp_updated)
		p->envp_updated = f_free_envp(p->envp_updated);
	free(p);
	return (NULL);
}
