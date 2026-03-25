/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:22:51 by slambert          #+#    #+#             */
/*   Updated: 2026/03/25 17:42:11 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	write_exit_error_and_free(char *str, t_data *data, int *saved_fds,
		int is_num)
{
	if (!is_num)
	{
		ft_putendl_fd_no_newline("minishell: exit: ", STDERR_FILENO);
		ft_putendl_fd_no_newline(str, STDERR_FILENO);
		ft_putendl_fd_no_newline(": numeric argument required\n",
			STDERR_FILENO);
	}
	sfbf_free_all(data);
	f_redir_restore(saved_fds, data);
}

void	b_exit(t_data *data, char **args, int *saved_fds)
{
	int	exit_no;
	int	is_num;
	int	i;

	is_num = 1;
	exit_no = 0;
	if (args[1])
	{
		i = -1;
		while (args[1][++i])
		{
			if (!ft_isdigit(args[1][i]))
			{
				is_num = 0;
				break ;
			}
		}
		if (is_num == 0)
			exit_no = 2;
		else
			exit_no = ft_atoi(args[1]);
	}
	write_exit_error_and_free(args[1], data, saved_fds, is_num);
	exit(exit_no);
}
