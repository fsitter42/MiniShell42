/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/31 23:05:48 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	f_path_exporter(t_cd *cd, t_data *data);
static void	f_deinit_cd(t_cd *cd);
static int	f_init_cd(t_cd *cd, t_data *data, char **args);

int	f_cd(t_data *data, char **args)
{
	t_cd	cd;

	if (f_init_cd(&cd, data, args) != EXIT_SUCCESS)
		return (f_deinit_cd(&cd), EXIT_FAILURE);
	if (chdir(cd.target) == -1)
		return (f_print_error(cd.target, strerror(errno)), free(cd.old_pwd), 1);
	cd.new_pwd = getcwd(NULL, 0);
	if (!cd.new_pwd)
		return (data->should_exit = 1, f_deinit_cd(&cd), EXIT_FAILURE);
	if (f_path_exporter(&cd, data) != EXIT_SUCCESS)
		return (f_deinit_cd(&cd), EXIT_FAILURE);
	return (f_deinit_cd(&cd), EXIT_SUCCESS);
}

static int	f_path_exporter(t_cd *cd, t_data *data)
{
	cd->export_str = ft_strjoin("OLDPWD=", cd->old_pwd);
	if (!cd->export_str)
		return (data->should_exit = 1, EXIT_FAILURE);
	if (f_export(data, (char *[]){"export", cd->export_str, NULL}) == 1)
		return (EXIT_FAILURE);
	free(cd->export_str);
	cd->export_str = ft_strjoin("PWD=", cd->new_pwd);
	if (!cd->export_str)
		return (data->should_exit = 1, EXIT_FAILURE);
	if (f_export(data, (char *[]){"export", cd->export_str, NULL}) == 1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	f_deinit_cd(t_cd *cd)
{
	if (cd->export_str)
		free(cd->export_str);
	if (cd->old_pwd)
		free(cd->old_pwd);
	if (cd->new_pwd)
		free(cd->new_pwd);
	if (!cd->has_target && cd->target)
		free(cd->target);
	return ;
}

static int	f_init_cd(t_cd *cd, t_data *data, char **args)
{
	cd->old_pwd = NULL;
	cd->new_pwd = NULL;
	cd->export_str = NULL;
	cd->target = NULL;
	cd->has_target = 0;
	if (args[1] && args[2] != NULL)
		return (f_print_error("cd", "too many arguments"), EXIT_FAILURE);
	//TODO @ Frido exit code ist falsch, sollte 2 sein
	if (args[1])
	{
		cd->target = args[1];
		cd->has_target = 1;
	}
	else
		cd->target = f_get_env_val(data->env->envp_lst, "HOME", data);
		
	//TODO @ Frido hier auf data->should_exit checken (wenn zb leeres 
	//environment) und ev. umbau auf ERROR_HARD etc. ?? bringt uns das 
	//was? wäre wsl viel zu ändern
	if (!cd->target)	
		return (EXIT_FAILURE);
	cd->old_pwd = getcwd(NULL, 0);
	if (!cd->old_pwd)
	{
		data->should_exit = 1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// int	f_cd(t_data *data, char **args)
// {
// 	char	*old_pwd;
// 	char	*new_pwd;
// 	char	*export_str;
// 	char	*target;

// 	if (args[2] != NULL)
// 		return (f_print_error("cd", "too many arguments"), EXIT_FAILURE);
// 	if (args[1])
// 		target = args[1];
// 	else
// 		target = f_get_env_val(data->env->envp_lst, "HOME", data);
// 	if (!target)
// 		return (EXIT_FAILURE);
// 	old_pwd = getcwd(NULL, 0);
// 	if (chdir(target) == -1)
// 		return (f_print_error(target, strerror(errno)), free(old_pwd),
// 			EXIT_FAILURE);
// 	new_pwd = getcwd(NULL, 0);
// 	export_str = ft_strjoin("OLDPWD=", old_pwd);
// 	if (f_export(data, (char *[]){"export", export_str, NULL}) == EXIT_FAILURE)
// 		return (free(export_str), free(old_pwd), free(new_pwd), EXIT_FAILURE);
// 	free(export_str);
// 	export_str = ft_strjoin("PWD=", new_pwd);
// 	if (f_export(data, (char *[]){"export", export_str, NULL}) == EXIT_FAILURE)
// 		return (free(export_str), free(old_pwd), free(new_pwd), EXIT_FAILURE);
// 	return (free(export_str), free(old_pwd), free(new_pwd), EXIT_SUCCESS);
// }

// TODOF getcwd protecten