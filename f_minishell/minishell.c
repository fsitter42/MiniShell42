/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:13:04 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/15 16:21:52 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit_code = 6;

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	(void)argc;
	(void)argv;
	ft_bzero(&data, sizeof(t_data));
	data.env = f_init_envp(envp);
	if (!data.env)
		return (1);
	data.cmds = ft_calloc(sizeof(t_cmd), 1);
	if (!data.cmds)
		return (f_destroy_envp(data.env), 1);
	data.cmds->in_fd = -1;
	data.cmds->out_fd = -1;
	data.cmds->cmd = "export";
	data.cmds->is_builtin = f_is_builtin(data.cmds->cmd);
	if (!data.cmds->is_builtin)
		data.cmds->path = f_path_handler(data.cmds->cmd, data.env->envp_updated);
	data.cmds->args = (char *[]){data.cmds->cmd,"xxx=xxx", NULL};
	f_exec_pipeline(&data, data.cmds);
	data.cmds->args = (char *[]){data.cmds->cmd, NULL};
	f_exec_pipeline(&data, data.cmds);
	if (data.cmds->path)
		free(data.cmds->path);
	free(data.cmds);
	data.env = f_destroy_envp(data.env);
	printf("exit code: %i\n", g_last_exit_code);
	return (0);
}

/*
	compile with:

	cc minishell.c ./envp/f_envp_to_lst.c envp/f_lst_to_new_envp.c  envp/f_envp_init_destroy.c builtin/env/f_env.c builtin/export/f_export_no_key.c builtin/export/f_export_key.c builtin/unset/f_unset.c builtin/export/f_export.c builtin/cd/f_cd.c builtin/echo/f_echo.c builtin/pwd/f_pwd.c execution/pathfinder/f_pathfinder.c execution/pathfinder/f_pathhandler.c ../includes/Libfs/libft.a -g && ./a.out


*/

/*
	here:
	execution loop
	builtin checker 

*/

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_data  data;
// 	ft_bzero(&data, sizeof(t_data));
	
// 	//t_envp	*data;

// 	data.env = f_init_envp(envp);
// 	if (!data.env)
// 		return (-1);
	
	
// 	char *export[] = {"env", NULL};
// 	g_last_exit_code = f_env(&data, export);
// 	printf("\n\n\n");
// 	char *add_y[] = {"export", "yyyy=halo du", NULL};
// 	g_last_exit_code = f_export(&data, add_y);

// 	char *add_z[] = {"export", "zzz=halo", "du", NULL};
// 	g_last_exit_code = f_export(&data, add_z);
	
	
// 	g_last_exit_code = f_env(&data, export);
// 	printf("\n\n\n");
// 	char *unset[] = {"export", "yyyy", NULL};
// 	printf("\n\nssssssssssssssssssssssssXXXX\n\n");
	
// 	g_last_exit_code = f_unset(&data, unset);
// 	g_last_exit_code = f_env(&data, export);
// 	printf("\n\n\n");
// 	// f_print_export(data->envp_lst);
// 	// printf("\n\n\n");
// 	char *path = "..";
// 	char *cd[] = {"cd", path};
// 	g_last_exit_code = f_pwd(&data, cd);
// 	data.env = f_destroy_envp(data.env);
// 	printf("%i\n", g_last_exit_code);
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_data  data;
// 	ft_bzero(&data, sizeof(t_data));
	
// 	//t_envp	*data;

// 	data.env = f_init_envp(envp);
// 	if (!data.env)
// 		return (-1);
	
	
// 	char *export[] = {"export", NULL};
// 	g_last_exit_code = f_export(&data, export);
// 	printf("\n\n\n");
// 	char *add_y[] = {"export", "yyyy=halo du", NULL};
// 	g_last_exit_code = f_export(&data, add_y);

// 	char *add_z[] = {"export", "zzz=halo du", NULL};
// 	g_last_exit_code = f_export(&data, add_z);
	
	
// 	g_last_exit_code = f_export(&data, export);
// 	printf("\n\n\n");
// 	char *unset[] = {"export", "yyyy", NULL};
// 	printf("\n\nssssssssssssssssssssssssXXXX\n\n");
// 	g_last_exit_code = f_export(&data, unset);
// 	//g_last_exit_code = f_unset(&data, unset);
// 	g_last_exit_code = f_export(&data, export);
// 	printf("\n\n\n");
// 	// f_print_export(data->envp_lst);
// 	// printf("\n\n\n");
// 	data.env = f_destroy_envp(data.env);
// 	printf("%i\n", g_last_exit_code);
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_envp	*data;

// 	data = f_init_envp(envp);
// 	if (!data)
// 		return (-1);
// 	f_print_env(data->envp_lst);
// 	printf("\n\n\n");
// 	f_lst_add_one(data->envp_lst, "zzz", "zzz");
// 	f_print_env(data->envp_lst);
// 	printf("\n\n\n");
// 	f_lst_del_one(&data->envp_lst, "zzz");
// 	f_print_env(data->envp_lst);
// 	printf("\n\n\n");
// 	data = f_destroy_envp(data);
// }

/*
 *
 * diese funktion in eine f_init_data() funktion verwandeln
 * malloc verwenden statt bzero
 * befüllen
 * und als data returnen
 *
 *
 * dann eine f_free_data() bauen mit den frees
 *
 *
 * dann die builtins die ich schon damit bauen kann bauen
 *
 *
 * 	cd builtin
 * 	mkdir env
 * 	touch env.h
 * 	touch env.c
 * 	include minishell reinhauen und probieren und dann für export
 *
 *
 *
 * jetzt für unset :D
 *
 *
 * im here:
 *

	* da jetzt alles was bis jetzt fertig ist in den richtigen ordnern ist kann ich die builtins bauen
 *
 * compile with
	* cc minishell.c ./envp/f_envp_to_lst.c envp/f_lst_to_new_envp.c  envp/f_envp_init_destroy.c builtin/env/f_env.c builtin/export/f_export_no_key.c builtin/export/f_export_key.c builtin/unset/f_unset.c builtin/export/f_export.c builtin/cd/f_cd.c builtin/echo/f_echo.c builtin/pwd/f_pwd.c ../includes/Libfs/libft.a
 *
 */