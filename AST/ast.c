/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:57 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 17:00:17 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* here we want to create a second linked list that consumes the token linked list and creates the exact
*  structure that we need in order to be able to execute the commands and create the pipes needed. 
*  we have to find a logic that extracts the correct amount of words that form one command (each flag
*  is a new word). 
*  furthermore, the pipes, <, > etc. define the redirection of the pipes needed.
*/
void create_command_list(t_token *token_list)
{
	
}
