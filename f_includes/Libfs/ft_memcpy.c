/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:41:01 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:55:29 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n);

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*mydest;
	unsigned char	*mysrc;
	size_t			i;

	i = 0;
	mydest = (unsigned char *)dest;
	mysrc = (unsigned char *)src;
	while (n > i)
	{
		mydest[i] = mysrc[i];
		i++;
	}
	return (dest);
}
