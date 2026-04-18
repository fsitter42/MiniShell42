/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_redirection.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:51:27 by slambert          #+#    #+#             */
/*   Updated: 2026/04/18 14:58:28 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//only used for the delimiter (we can't just blindly expand the delimiter
//because $asdf would be expanded like that. $asdf is a valid delimiter
//and must not be expanded)
static char	*strip_syntactic_quotes(char *word)
{
	char	*out;
	int		i;
	int		j;
	int		quote_status;

	out = ft_calloc(ft_strlen(word) + 1, sizeof(char));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	quote_status = DEFAULT_Q;
	while (word[i])
	{
		if (consume_syntactic_quote(word[i], &quote_status))
		{
			i++;
			continue ;
		}
		out[j++] = word[i++];
	}
	return (out);
}

// returns 0 is the redirection target is not valid
int	is_valid_redirection_target(t_token *redir_token)
{
	if (!redir_token || !redir_token->next)
		return (0);
	if (redir_token->next->type != WORD
		&& redir_token->next->type != WORD_AFTER_HEREDOC)
		return (0);
	if (!redir_token->next->str)
		return (0);
	return (1);
}

static void	add_redir_to_redir_list(t_redir *new_redir, t_cmd *cmd)
{
	t_redir	*cur;

	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		cur = cmd->redirs;
		while (cur->next)
			cur = cur->next;
		cur->next = new_redir;
	}
}

static int	next_redir_id(void)
{
	static int	id;

	id++;
	if (id <= 0)
		id = 1;
	return (id);
}

int	add_r_t_c(t_cmd *cmd, int type, char *str, char *delimiter)
{
	t_redir	*new_redir;

	new_redir = ft_calloc(sizeof(t_redir), 1);
	if (!new_redir)
		return (ERROR_HARD);
	if (delimiter)
	{
		new_redir->delimiter = strip_syntactic_quotes(delimiter);
		if (!new_redir->delimiter)
			return (free(new_redir), ERROR_HARD);
	}
	if (str)
	{
		new_redir->file = ft_strdup(str);
		if (!new_redir->file)
			return (free(new_redir->delimiter), free(new_redir), ERROR_HARD);
	}
	new_redir->type = type;
	new_redir->id = next_redir_id();
	add_redir_to_redir_list(new_redir, cmd);
	return (RET_OK);
}
