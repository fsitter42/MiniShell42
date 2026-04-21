/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/21 13:34:31 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

// int	f_pwd(t_data *data)
// {
// 	char	*cwd;

// 	(void) data;
// 	cwd = getcwd(NULL, 0);
// 	if (!cwd)
// 	{
// 		return (EXIT_FAILURE);
// 	}
// 	ft_putendl_fd(cwd, 1);
// 	free(cwd);
// 	return (EXIT_SUCCESS);
// }

int	f_pwd(t_data *data)
{
	char	*cwd;
	char	*pwd_env;
	int		nn;

	nn = 0;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putendl_fd(cwd, 1);
		free(cwd);
		return (EXIT_SUCCESS);
	}
	pwd_env = f_get_env_val_with_check(data->env->envp_lst, "PWD", data, &nn);
	if (pwd_env)
	{
		ft_putendl_fd(pwd_env, 1);
		return (EXIT_SUCCESS);
	}
	ft_putendl_fd("minishell: pwd: PWD not set", 2);
	return (EXIT_FAILURE);
}