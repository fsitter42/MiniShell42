/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:22:45 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:58:04 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned int n, const char format);

int	ft_puthex(unsigned int n, const char format)
{
	int		len;
	int		is_valid;
	char	*base;

	base = "0123456789ABCDEF";
	if (format == 'x')
		base = "0123456789abcdef";
	len = 0;
	if (n > 15)
		len += ft_puthex((n / 16), format);
	is_valid = ft_putchar(base[n % 16]);
	if (is_valid == -1)
		return (is_valid);
	return (len + is_valid);
}
