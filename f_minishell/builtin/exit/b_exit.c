/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:22:51 by slambert          #+#    #+#             */
/*   Updated: 2026/03/28 16:49:27 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	write_exit_error_and_free(t_cmd *cmd, t_data *data, int *saved_fds,
		int is_num)
{
	if (cmd->is_first)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!is_num)
	{
		ft_putendl_fd_no_nl("minishell: exit: ", STDERR_FILENO);
		ft_putendl_fd_no_nl(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd_no_nl(": numeric argument required\n", STDERR_FILENO);
	}
	sfbf_free_all(data);
	f_redir_restore(saved_fds, data);
}

// is_num holds the info whether the argument holds only digits
int	set_is_num(char **args)
{
	int	i;
	int	is_num;

	is_num = 1;
	i = 0;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	if (args[1][i] == '\0')
		is_num = 0;
	while (is_num && args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			is_num = 0;
			break ;
		}
		i++;
	}
	return (is_num);
}

void	too_many_args_handler(t_data *data, t_cmd *cmd)
{
	if (cmd->is_first)
		ft_putendl_fd("exit", STDOUT_FILENO);
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	data->last_exit_code = 2;
	data->should_exit = 0;
}

void	b_exit(t_data *data, t_cmd *cmd, int *saved_fds)
{
	int	exit_no;
	int	is_num;

	is_num = 0;
	exit_no = 0;
	if (cmd->args[1] && cmd->args[2])
	{
		too_many_args_handler(data, cmd);
		return ;
	}
	if (cmd->args[1])
	{
		is_num = set_is_num(cmd->args);
		if (is_num == 0)
		{
			write_exit_error_and_free(cmd, data, saved_fds, 0);
			exit(2);
		}
		else
			exit_no = ft_atoi(cmd->args[1]);
	}
	sfbf_free_all(data);
	f_redir_restore(saved_fds, data);
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit(exit_no);
}
