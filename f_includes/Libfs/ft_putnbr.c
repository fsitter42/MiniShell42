/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:08:50 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:58:36 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int nb);

int	ft_putnbr(int nb)
{
	int			len;
	int			is_valid;
	long long	n;

	len = 0;
	n = nb;
	if (n < 0)
	{
		len++;
		ft_putchar('-');
		n *= -1;
	}
	if (n > 9)
		len += ft_putnbr(n / 10);
	is_valid = ft_putchar(n % 10 + 48);
	if (is_valid == -1)
		return (is_valid);
	return (len + is_valid);
}
