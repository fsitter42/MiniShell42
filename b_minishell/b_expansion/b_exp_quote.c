/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exp_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 14:32:34 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 19:53:49 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_handler(int quote_status, char c)
{
	if (quote_status == DEFAULT_Q)
	{
		if (c == '\'')
			return (IN_SINGLE_Q);
		if (c == '\"')
			return (IN_DOUBLE_Q);
	}
	else if ((quote_status == IN_SINGLE_Q && c == '\'')
		|| (quote_status == IN_DOUBLE_Q && c == '\"'))
		return (DEFAULT_Q);
	return (quote_status);
}

/*
 * removes a quote character when that quote changes the quote state
 * (opens or closes the current mode)
 * returns 1 if a quote has been consumed
 */
int	consume_syntactic_quote(char c, int *quote_status)
{
	int	next_quote_status;

	next_quote_status = quote_handler(*quote_status, c);
	if ((c == '\'' || c == '"') && next_quote_status != *quote_status)
	{
		*quote_status = next_quote_status;
		return (1);
	}
	return (0);
}
