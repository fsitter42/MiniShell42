/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/21 11:02:30 by fsitter          ###   ########.fr       */
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
		return (f_print_error(cd.target, strerror(errno)), 1);
	cd.new_pwd = getcwd(NULL, 0);
	if (!cd.new_pwd)
		return (f_print_error(cd.target, strerror(errno)), f_deinit_cd(&cd),
			EXIT_FAILURE);
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
	if (cd->new_pwd)
		free(cd->new_pwd);
	if (!cd->has_target && cd->target)
		free(cd->target);
	return ;
}

static void	f_init_init_cd(t_cd *cd)
{
	cd->new_pwd = NULL;
	cd->export_str = NULL;
	cd->target = NULL;
	cd->has_target = 0;
	return ;
}

static int	f_init_cd(t_cd *cd, t_data *data, char **args)
{
	int	val_not_null;

	val_not_null = 0;
	f_init_init_cd(cd);
	if (args[1] && args[2] != NULL)
		return (f_print_error("cd", "too many arguments"), EXIT_FAILURE);
	if (args[1])
	{
		cd->target = args[1];
		cd->has_target = 1;
	}
	else
		cd->target = f_get_env_val_with_check(data->env->envp_lst, "HOME", data,
				&val_not_null);
	if (!cd->target)
	{
		if (val_not_null != 1)
			f_print_error("cd", "HOME not set");
		return (EXIT_FAILURE);
	}
	if (!getcwd(cd->old_pwd, sizeof(cd->old_pwd)))
		cd->old_pwd[0] = '\0';
	return (EXIT_SUCCESS);
}
