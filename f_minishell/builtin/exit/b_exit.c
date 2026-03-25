/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:22:51 by slambert          #+#    #+#             */
/*   Updated: 2026/03/25 18:00:23 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

//TODO exit 24 asd should print minishell: exit: too many arguments
//and NOT actually exit
static void	write_exit_error_and_free(char *str, t_data *data, int *saved_fds,
		int is_num)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
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
		i = 0;
		if (args[1][i] == '+' || args[1][i] == '-')
			i++;
		if (args[1][i] == '\0')
			is_num = 0;
		while (is_num && args[1][i])
		{
			if (!ft_isdigit((unsigned char)args[1][i]))
			{
				is_num = 0;
				break ;
			}
			i++;
		}
		if (is_num == 0)
			exit_no = 2;
		else
			exit_no = ft_atoi(args[1]);
	}
	write_exit_error_and_free(args[1], data, saved_fds, is_num);
	exit(exit_no);
}
