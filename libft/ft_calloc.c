/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:20:58 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:56:12 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size);

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	bytesize;
	void	*pointer;
	size_t	mione;

	mione = -1;
	if (nmemb <= 0 || size <= 0)
	{
		pointer = malloc(0);
		if (!pointer)
			return (NULL);
		return (pointer);
	}
	if (mione / size < nmemb)
		return (NULL);
	bytesize = nmemb * size;
	pointer = malloc(bytesize);
	if (!pointer)
		return (NULL);
	ft_memset(pointer, 0, bytesize);
	return (pointer);
}
