/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:54:39 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:55:20 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n);

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	ctr;
	size_t			i;

	i = 0;
	str = (unsigned char *)s;
	ctr = (unsigned char)c;
	while (n > i)
	{
		str[i] = ctr;
		i++;
	}
	return (s);
}
