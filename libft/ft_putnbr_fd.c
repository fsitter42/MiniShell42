/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:43:44 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:55:07 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	long long	number;

	number = n;
	if (number < 0)
	{
		write(fd, "-", 1);
		number *= -1;
	}
	while (number > 9)
	{
		ft_putnbr_fd(number / 10, fd);
		number %= 10;
	}
	number = number + 48;
	write(fd, &number, 1);
}
