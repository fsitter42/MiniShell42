/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_envp.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 22:30:21 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/28 12:11:48 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

// f_envp_to_lst.c
t_envl	*f_copy_env(char **envp);
t_envl	*f_new_node(void);
t_envl	*f_free_env_list(t_envl *head);
int		f_fill_node(t_envl *node, char *env_str);

// f_lst_to_new_envp.c
int		f_update_envp(t_envp *data, t_data *bigdata);
char	**f_free_envp(char **envp);
size_t	f_lst_len_active(t_envl *list);
char	**f_envp_lst_to_p(t_envl *list, size_t keylen, size_t vallen, size_t i);

// f_envp_init_destroy.c
t_envp	*f_init_envp(char **envp);
t_envp	*f_destroy_envp(t_envp *p);

#endif