/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 11:57:49 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/18 12:06:07 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_digit(long long *num, int neg, long long limit, char c)
{
	if (!neg && *num > (limit - (c - '0')) / 10)
		return (1);
	if (neg && *num < (limit + (c - '0')) / 10)
		return (1);
	if (!neg)
		*num = *num * 10 + (c - '0');
	else
		*num = *num * 10 - (c - '0');
	return (0);
}

int	is_overflow(char *arg)
{
	long long	num;
	long long	limit;
	int			neg;
	int			i;

	i = 0;
	neg = 0;
	num = 0;
	while (arg[i] == ' ' || arg[i] == '\t')
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		neg = (arg[i++] == '-');
	if (neg)
		limit = LLONG_MIN;
	else
		limit = LLONG_MAX;
	while (ft_isdigit(arg[i]))
	{
		if (check_digit(&num, neg, limit, arg[i]))
			return (1);
		i++;
	}
	return (0);
}

long long	f_atoll(char *arg)
{
	long long	num;
	int			neg;
	int			i;

	i = 0;
	neg = 0;
	num = 0;
	while (arg[i] == ' ' || arg[i] == '\t')
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		neg = (arg[i++] == '-');
	while (ft_isdigit(arg[i]))
	{
		if (neg)
			num = num * 10 - (arg[i] - '0');
		else
			num = num * 10 + (arg[i] - '0');
		i++;
	}
	return (num);
}
