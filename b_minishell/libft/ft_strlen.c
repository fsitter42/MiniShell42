/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a600 <a600@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:20:17 by slambert          #+#    #+#             */
/*   Updated: 2026/04/15 23:25:24 by a600             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
/* 
#include <stdio.h>
#include <string.h>

int main (void)
{
	char str[] = "hrsnbth";
	printf("%zu\n", strlen(str));
	printf("%zu\n", ft_strlen(str));
} */