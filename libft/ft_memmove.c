/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:48:46 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:58:26 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n);

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*mydest;
	const char	*mysrc;
	size_t		i;

	mydest = dest;
	mysrc = src;
	i = 0;
	if (mydest == mysrc || n == 0)
		return (dest);
	while (i < n)
	{
		if (mydest > mysrc && mysrc + n > mydest)
			mydest[n - i - 1] = mysrc[n - i - 1];
		else
			mydest[i] = mysrc[i];
		i++;
	}
	return (dest);
}
