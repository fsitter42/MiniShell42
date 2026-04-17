/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:22:51 by slambert          #+#    #+#             */
/*   Updated: 2026/04/17 14:25:43 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	write_numeric_error(char *arg)
{
	ft_putendl_fd_no_nl("minishell: exit: ", STDERR_FILENO);
	ft_putendl_fd_no_nl(arg, STDERR_FILENO);
	ft_putendl_fd_no_nl(": numeric argument required\n", STDERR_FILENO);
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

static void	write_error_and_set_exit_code(t_data *data, t_cmd *cmd)
{
	write_numeric_error(cmd->args[1]);
	data->last_exit_code = 2;
	data->should_exit = 1;
}

void	too_many_args_handler(t_data *data, t_cmd *cmd)
{
	int is_num;
	
	is_num = set_is_num(cmd->args);
	if (is_num == 0)
	{
		write_error_and_set_exit_code(data, cmd);
		return ;
	}
	if (cmd->is_first)
		ft_putendl_fd("exit", STDOUT_FILENO);
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	if (data->last_exit_code == 0 && !data->e_has_been_set)
		data->last_exit_code = 1;
	data->should_exit = 0;
}

static int skip_space_zeros_and_a_sign(char *arg)
{
	int i;

	i = -1;
	while (arg[++i])
	{
		if ()
	}
}

static int is_ll_overflow (char *arg)
{
	long long num;
	int i;
	
	i = 0;
	if ()
	if (...)
		return 1;
	return 0;
}

static int is_ll_underflow (char *arg)
{
	if (...)
		return 1;
	return 0;
}

//TODO return was gscheids
long long create_exit_code (char *arg, t_data *data, t_cmd* cmd)
{
	if (is_ll_overflow(arg) || is_ll_underflow(arg))
	{
		//on overflow we have to write bash: exit: 555555555555555555555555555555555555555555555555555: numeric argument required
		//AND exit with status code 2
		write_error_and_set_exit_code(data, cmd);
		return 2;
	}
	else
		return (unsigned char)ft_atoi(arg);
}

void	b_exit(t_data *data, t_cmd *cmd)
{
	int			exit_no;
	int			is_num;

	is_num = 0;
	exit_no = 0;
	if (cmd->args[1] && cmd->args[2])
	{
		too_many_args_handler(data, cmd);
		return ;
	}
	if (cmd->is_first)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd->args[1])
	{
		
		is_num = set_is_num(cmd->args);
		if (is_num == 0)
		{
			write_error_and_set_exit_code(data, cmd);
			return ;
		}
		else
			exit_no = create_exit_code(cmd->args[1], data, cmd);
		//	exit_no = (unsigned char)ft_atoi(cmd->args[1]);
	}
	data->last_exit_code = exit_no;
	data->should_exit = 1;
}
