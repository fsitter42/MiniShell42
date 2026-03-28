/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_export.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:46:00 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/28 12:02:53 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

// f_export_no_key.c
int		f_print_export(t_envl *original_list);
t_envl	*f_copy_list(t_envl *head);
void	f_sort_env_list(t_envl *head);
int		f_strcmp(const char *s1, const char *s2);

// f_export_key.c
int		f_lst_add_one(t_envl *list, char *key, char *val);
void	f_free_node(t_envl *node);
int		f_valid_identifier(char *s);
int	f_export_with_key(t_envl *envl, char *s, t_data *data);

// f_export.c
int		f_export(t_data *data, char **args);

#endif