/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:21:50 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:57:15 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putptr(unsigned long nb);

int	ft_putptr(unsigned long nb)
{
	int		len;
	int		is_valid;
	char	*base;

	base = "0123456789abcdef";
	len = 0;
	if (nb > 15)
		len += ft_putptr((nb / 16));
	is_valid = ft_putchar(base[nb % 16]);
	if (is_valid == -1)
		return (is_valid);
	return (len + is_valid);
}
