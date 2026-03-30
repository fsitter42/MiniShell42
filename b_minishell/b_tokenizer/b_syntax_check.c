/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_syntax_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 18:32:42 by slambert          #+#    #+#             */
/*   Updated: 2026/03/30 18:34:22 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*b_token_type_to_str(int type)
{
	if (type == PIPE)
		return ("|");
	if (type == HEREDOC)
		return ("<<");
	if (type == REDIR_IN)
		return ("<");
	if (type == REDIR_APPEND)
		return (">>");
	if (type == REDIR_OUT)
		return (">");
	if (type == WORD_AFTER_HEREDOC || type == WORD)
		return ("word");
	return ("newline");
}

static int	b_report_syntax_error(t_data *data, char *unexpected)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(unexpected, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	data->last_exit_code = 2;
	return (ERROR_SOFT);
}

int	b_validate_token_sequence(t_token *token, t_data *data)
{
	t_token	*next;
	t_token	*prev;

	prev = NULL;
	while (token)
	{
		next = token->next;
		if (token->type == PIPE)
		{
			if (!prev)
				return (b_report_syntax_error(data, "|"));
			if (!next)
				return (b_report_syntax_error(data, "newline"));
			if (next->type == PIPE)
				return (b_report_syntax_error(data, "|"));
		}
		if (is_token_type_redirection(token))
		{
			if (!next)
				return (b_report_syntax_error(data, "newline"));
			if (next->type != WORD && next->type != WORD_AFTER_HEREDOC)
				return (b_report_syntax_error(data,
						b_token_type_to_str(next->type)));
			token = next;
		}
		prev = token;
		token = token->next;
	}
	return (RET_OK);
}
