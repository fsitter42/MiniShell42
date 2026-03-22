/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/22 16:02:09 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	f_cd(t_data *data, char **args)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*export_str;
	char	*target;

	if (args[1])
		target = args[1];
	else
		target = f_get_env_val(data->env->envp_lst, "HOME");
	if (!target)
		return (EXIT_FAILURE);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		f_print_error(target, strerror(errno));
		free(old_pwd);
		return (EXIT_FAILURE);
	}
	new_pwd = getcwd(NULL, 0);
	export_str = ft_strjoin("OLDPWD=", old_pwd);
	f_export(data, (char *[]){"export", export_str, NULL});
	free(export_str);
	export_str = ft_strjoin("PWD=", new_pwd);
	f_export(data, (char *[]){"export", export_str, NULL});
	free(export_str);
	free(old_pwd);
	free(new_pwd);
	return (EXIT_SUCCESS);
}
