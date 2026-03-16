/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_unset.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:48:03 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/11 12:34:37 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNSET_H
# define UNSET_H

// f_unset.c
int		f_unset(t_data *data, char **args);
int		f_lst_del_one(t_envl **list, char *key);
char	*f_get_env_val(t_envl *list, char *key);

#endif