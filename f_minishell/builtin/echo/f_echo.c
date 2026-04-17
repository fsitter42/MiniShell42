/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:49:53 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/17 12:06:20 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	f_nflag(char **args, size_t *i);

int	f_echo(t_data *data, char **args)
{
	int		ret;
	size_t	i;
	int		n_flag;

	ret = EXIT_SUCCESS;
	i = 1;
	if (!data || !args || !args[0])
		return (EXIT_FAILURE);
	n_flag = f_nflag(args, &i);
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (ret);
}

static int	f_nflag(char **args, size_t *i)
{
	size_t	j;

	while (args[*i] && args[*i][0] == '-')
	{
		j = 1;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] != '\0')
			break ;
		(*i)++;
	}
	return (*i > 1);
}
