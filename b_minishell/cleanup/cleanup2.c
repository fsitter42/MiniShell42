/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:41:03 by slambert          #+#    #+#             */
/*   Updated: 2026/03/22 12:45:08 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit_function(char *error_msg)
{
	if (error_msg)
		printf("%s\n", error_msg);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	exit(0);
}
