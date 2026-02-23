/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:54:32 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 16:58:06 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void cleanup_token_list(t_token *token_list)
{
	t_token	*temp = NULL;
	
	while (token_list)
	{
		temp = token_list->next;
		free (token_list->str);
		free (token_list);
		token_list = temp;
	}
}

void my_exit_function()
{
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	exit(0);
}
