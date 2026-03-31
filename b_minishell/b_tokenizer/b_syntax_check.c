/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_syntax_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 18:32:42 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 11:49:41 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*b_token_to_str(int type)
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
	return ("\\n");
}

static int	b_rep_s_err(t_data *data, char *unexpected)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(unexpected, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	data->last_exit_code = 2;
	return (ERROR_SOFT);
}

int	b_validation_token_handler(t_data *data, t_token *prev, t_token *next)
{
	if (!prev)
		return (b_rep_s_err(data, "|"));
	if (!next)
		return (b_rep_s_err(data, "\\n"));
	if (next->type == PIPE)
		return (b_rep_s_err(data, "|"));
	return (ERROR_HARD);
}

int	b_validate_token_sequence(t_token *token, t_data *data)
{
	t_token	*prev;
	t_token	*next;

	prev = NULL;
	while (token)
	{
		next = token->next;
		if (token->type == PIPE)
		{
			if (!prev || !next || next->type == PIPE)
				return (b_validation_token_handler(data, prev, next));
		}
		if (is_token_type_redirection(token))
		{
			if (!next)
				return (b_rep_s_err(data, "\\n"));
			if (next->type != WORD && next->type != WORD_AFTER_HEREDOC)
				return (b_rep_s_err(data, b_token_to_str(next->type)));
			token = next;
		}
		prev = token;
		token = token->next;
	}
	return (RET_OK);
}
