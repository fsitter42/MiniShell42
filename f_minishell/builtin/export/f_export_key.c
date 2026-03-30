/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_export_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 01:11:54 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/30 11:04:45 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	f_free_key_n_val(char *key, char *val);

void	f_free_node(t_envl *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->val)
		free(node->val);
	free(node);
}

int	f_lst_add_one(t_envl *list, char *key, char *val)
{
	t_envl	*new;

	if (!list || !key)
		return (0);
	while (list->next)
		list = list->next;
	new = f_new_node();
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	if (!new->key)
		return (f_free_node(new), 0);
	if (val)
	{
		new->val = ft_strdup(val);
		if (!new->val)
			return (f_free_node(new), 0);
	}
	list->next = new;
	return (1);
}

int	f_valid_identifier(char *s)
{
	size_t	i;
	int		ret;

	ret = 0;
	i = 1;
	if (!s || !s[0] || (!ft_isalpha(s[0]) && s[0] != '_'))
		ret = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			ret = 1;
		i++;
	}
	if (ret == 1)
	{
		ft_putstr_fd("minishell: export: `", 2);
		if (s)
			ft_putstr_fd(s, 2);
		else
			ft_putstr_fd("", 2);
		ft_putstr_fd("\': not a valid identifier\n", 2);
	}
	return (ret);
}

int	f_export_with_key(t_envl *envl, char *s, t_data *data, int *err)
{
	char	*key;
	char	*val;
	char	*eq;

	*err = 0;
	if (f_valid_identifier(s))
		return (*err = 2, 1);
	val = NULL;
	eq = ft_strchr(s, '=');
	if (!eq)
		key = ft_strdup(s);
	else
	{
		key = ft_substr(s, 0, eq - s);
		val = ft_strdup(eq + 1);
	}
	if (!key || (eq && !val))
		return (f_free_key_n_val(key, val), 1);
	if (!val)
		val = f_get_env_val(envl, key, data);
	f_lst_del_one(&envl, key);
	if (!f_lst_add_one(envl, key, val))
		return (f_free_key_n_val(key, val), 1);
	f_free_key_n_val(key, val);
	return (0);
}

void	f_free_key_n_val(char *key, char *val)
{
	if (key)
		free(key);
	if (val)
		free(val);
	return ;
}
