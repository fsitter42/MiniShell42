/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putuint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:21:55 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:57:23 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putuint(unsigned int nb);

int	ft_putuint(unsigned int nb)
{
	int	len;
	int	is_valid;

	len = 0;
	if (nb > 9)
		len += ft_putuint(nb / 10);
	is_valid = ft_putchar(nb % 10 + 48);
	if (is_valid == -1)
		return (is_valid);
	return (len + is_valid);
}
