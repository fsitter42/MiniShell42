/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd_no_newline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:31:31 by slambert          #+#    #+#             */
/*   Updated: 2026/03/26 12:53:49 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd_no_nl(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}