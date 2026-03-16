/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_export_no_key.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:45:56 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/16 14:03:10 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_print_export(t_envl *original_list)
{
	t_envl	*copy;
	t_envl	*tmp;

	if (!original_list)
		return (1);
	copy = f_copy_list(original_list);
	if (!copy)
		return (1);
	f_sort_env_list(copy);
	tmp = copy;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (tmp->val != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->val, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		tmp = tmp->next;
	}
	f_free_env_list(copy);
	return (0);
}

t_envl	*f_copy_list(t_envl *head)
{
	t_envl	*new_head;
	t_envl	*curr;
	t_envl	*new;

	new_head = NULL;
	curr = NULL;
	while (head)
	{
		new = f_new_node();
		if (!new)
			return (f_free_env_list(new_head));
		new->key = ft_strdup(head->key);
		if (head->val)
			new->val = ft_strdup(head->val);
		if (!new->key || (head->val && !new->val))
			return (f_free_env_list(new), f_free_env_list(new_head));
		if (!new_head)
			new_head = new;
		else
			curr->next = new;
		curr = new;
		head = head->next;
	}
	return (new_head);
}

void	f_sort_env_list(t_envl *head)
{
	t_envl	*i;
	t_envl	*j;
	char	*tmp_key;
	char	*tmp_val;

	i = head;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (f_strcmp(i->key, j->key) > 0)
			{
				tmp_key = i->key;
				i->key = j->key;
				j->key = tmp_key;
				tmp_val = i->val;
				i->val = j->val;
				j->val = tmp_val;
			}
			j = j->next;
		}
		i = i->next;
	}
}

int	f_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
