/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:00:30 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/17 23:59:16 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_EXE_H
# define F_EXE_H

// f_pathfinder.c
char	*f_find_path(char *cmd, char **envp, int *err);
void	f_validate_path(char *path, int *err);

// f_pathhandler.c
char	*f_path_handler(t_data *data, char *cmd, char **envp);
void	f_print_error(char *context, char *msg);

// f_singlecmd.c
void	f_exec_cmd(t_data *data, t_cmd *cmd, char **envp);
int		f_is_builtin(char *cmd);
void	f_exec_builtin(t_cmd *cmd, t_data *data);

// f_exe.c
int		f_exec_pipeline(t_data *data, t_cmd *cmds);

// f_exe2.c
int	f_open_redirections(t_data *data, t_cmd *cmd);

#endif