/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:19:37 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:55:41 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n);

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	ctr;

	str = (unsigned char *)s;
	ctr = (unsigned char)c;
	while (n > 0)
	{
		if (*str == ctr)
		{
			return (str);
		}
		str++;
		n--;
	}
	return (NULL);
}
