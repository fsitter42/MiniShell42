/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_envp_to_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:11:44 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/30 11:48:13 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_envl	*f_copy_env(char **envp)
{
	t_envl	*head;
	t_envl	*current;
	t_envl	*new;
	size_t	i;

	i = 0;
	if (!envp || !envp[0])
		return (NULL);
	head = NULL;
	current = NULL;
	while (envp[i])
	{
		new = f_new_node();
		if (!new)
			return (f_free_env_list(head));
		if (!f_fill_node(new, envp[i]))
			return (free(new), f_free_env_list(head));
		if (head == NULL)
			head = new;
		else
			current->next = new;
		current = new;
		i++;
	}
	return (head);
}

t_envl	*f_new_node(void)
{
	t_envl	*node;

	node = ft_calloc(sizeof(t_envl), 1);
	if (!node)
		return (NULL);
	node->key = NULL;
	node->val = NULL;
	node->next = NULL;
	return (node);
}

t_envl	*f_free_env_list(t_envl *head)
{
	t_envl	*next;

	if (!head)
		return (NULL);
	while (head)
	{
		next = head->next;
		if (head->key)
			free(head->key);
		if (head->val)
			free(head->val);
		free(head);
		head = next;
	}
	return (NULL);
}

int	f_fill_node(t_envl *node, char *env_str)
{
	char	*first_eq_sign;

	first_eq_sign = ft_strchr(env_str, '=');
	if (!first_eq_sign)
		return (0);
	node->key = ft_substr(env_str, 0, first_eq_sign - env_str);
	if (!node->key)
		return (0);
	node->val = ft_strdup(first_eq_sign + 1);
	if (!node->val)
	{
		free(node->key);
		return (0);
	}
	return (1);
}
