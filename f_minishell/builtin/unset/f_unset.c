/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 01:09:18 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/15 17:57:17 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_unset(t_data *data, char **args)
{
	int	ret;

	ret = EXIT_SUCCESS;
	if (!data || !args || !args[0])
		return (EXIT_FAILURE);
	if (!args[1])
		return (EXIT_SUCCESS);
	else
	{
		ret = f_lst_del_one(&data->env->envp_lst, args[1]);
		ret = f_update_envp(data->env, data);
	}
	return (ret);
}

int	f_lst_del_one(t_envl **list, char *key)
{
	t_envl	*before;
	t_envl	*current;

	if (!list || !*list || !key)
		return (EXIT_FAILURE);
	before = NULL;
	current = *list;
	while (current && f_strcmp(key, current->key) != 0)
	{
		before = current;
		current = current->next;
	}
	if (current == NULL)
		return (EXIT_SUCCESS);
	if (before)
		before->next = current->next;
	else
		*list = current->next;
	f_free_node(current);
	return (EXIT_SUCCESS);
}

char	*f_get_env_val(t_envl *list, char *key, t_data *data)
{
	char	*ret;

	if (!list || !key)
		return (NULL);
	while (list)
	{
		if (f_strcmp(key, list->key) == 0)
		{
			//TODO do we have to safeguard against NULL here?
			//can list->val be NULL?
			ret = ft_strdup(list->val);
			if (!ret)
				data->should_exit = 1;
			return (ret);
		}
		list = list->next;
	}
	return (NULL);
}
