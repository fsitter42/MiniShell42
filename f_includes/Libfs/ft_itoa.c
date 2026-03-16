/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:20:57 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:55:44 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n);
int		wie_viele_stellen(long long number);

int	wie_viele_stellen(long long number)
{
	int	stellen;

	stellen = 0;
	if (number < 0)
	{
		number *= -1;
		stellen++;
	}
	if (number == 0)
		stellen++;
	while (number > 0)
	{
		number /= 10;
		stellen++;
	}
	return (stellen);
}

char	*ft_itoa(int n)
{
	long long	number;
	int			stellen;
	char		*itoa;

	number = n;
	stellen = wie_viele_stellen(n);
	itoa = malloc(sizeof(char) * stellen + 1);
	if (!itoa)
		return (0);
	itoa[stellen--] = '\0';
	if (number == 0)
		itoa[0] = 48;
	if (number < 0)
	{
		itoa[0] = '-';
		number *= -1;
	}
	while (number > 0)
	{
		itoa[stellen--] = number % 10 + 48;
		number /= 10;
	}
	return (itoa);
}
