/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_cd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:00:03 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/30 10:31:21 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

typedef struct s_cd t_cd;

typedef struct s_cd
{
	char	*old_pwd;
	char	*new_pwd;
	char	*export_str;
	char	*target;
    int     has_target;
}			t_cd;

// f_cd.c
int			f_cd(t_data *data, char **args);

#endif