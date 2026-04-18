/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_main_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 15:03:17 by slambert          #+#    #+#             */
/*   Updated: 2026/04/18 19:48:35 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_and_free_line(char *line)
{
	printf("minishell: missing quote\n");
	if (*line && !line_is_empty(line))
		add_history((const char *)line);
	free(line);
}

void	errno_and_exit_code_helper(t_data *data)
{
	if (errno != 0)
		data->last_exit_code = 1;
	ft_putendl_fd("exit", 1);
}
