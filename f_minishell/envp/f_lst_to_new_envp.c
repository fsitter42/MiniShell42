/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_lst_to_new_envp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 23:00:29 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:03:50 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_update_envp(t_envp *data)
{
	if (data->envp_updated)
		data->envp_updated = f_free_envp(data->envp_updated);
	data->envp_updated = f_envp_lst_to_p(data->envp_lst, 0, 0, 0);
	if (!data->envp_updated)
		return (1);
	return (0);
}

char	**f_free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
	envp = NULL;
	return (envp);
}

size_t	f_lst_len_active(t_envl *list)
{
	size_t	len;

	len = 0;
	if (!list)
		return (0);
	while (list)
	{
		if (list->val != NULL)
			len++;
		list = list->next;
	}
	return (len);
}

char	**f_envp_lst_to_p(t_envl *list, size_t keylen, size_t vallen, size_t i)
{
	char	**envp_updated;

	if (!list)
		return (NULL);
	envp_updated = ft_calloc(sizeof(char *), f_lst_len_active(list) + 1);
	if (!envp_updated)
		return (NULL);
	while (list)
	{
		if (list->val)
		{
			keylen = ft_strlen(list->key);
			vallen = ft_strlen(list->val);
			envp_updated[i] = ft_calloc((keylen + vallen + 2), sizeof(char));
			if (!envp_updated[i])
				return (f_free_envp(envp_updated));
			ft_memcpy(envp_updated[i], list->key, keylen);
			envp_updated[i][keylen] = '=';
			ft_memcpy(&envp_updated[i][keylen + 1], list->val, vallen);
			envp_updated[i][keylen + vallen + 1] = '\0';
			i++;
		}
		list = list->next;
	}
	return (envp_updated);
}
