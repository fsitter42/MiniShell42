/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_signal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:00:30 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/02 11:47:58 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_SIGNAL_H
# define F_SIGNAL_H

volatile sig_atomic_t g_signal_received = 0;

// f_signal.c
char	*f_find_path(char *cmd, char **envp, int *err, int *err2);


#endif