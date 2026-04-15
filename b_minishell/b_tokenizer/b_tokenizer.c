/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/04/15 13:54:58 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 *  checks if every opening quote has a dedicated closing
 *  bracket of the same type
 * returns 0 on success and 1 or 2 on failure
 */
int	quote_syntax_check(char *line)
{
	int	i;
	int	cur_status;

	i = -1;
	cur_status = 0;
	while (line[++i])
	{
		if (cur_status == 0)
		{
			if (line[i] == '\'')
				cur_status = 1;
			else if (line[i] == '\"')
				cur_status = 2;
		}
		else if ((cur_status == 1 && line[i] == '\'') || (cur_status == 2
				&& line[i] == '\"'))
			cur_status = 0;
	}
	return (cur_status);
}

int	word_handler(int i, char *line, t_token *list_start, int *quote_status)
{
	int		word_start;
	int		char_to_check;
	char	*word;
	int		quote_save;

	word_start = i;
	char_to_check = word_start + 1;
	quote_save = *quote_status;
	while (TRUE)
	{
		if (!is_part_of_word(line[char_to_check], quote_status))
			break ;
		quote_save = *quote_status;
		*quote_status = quote_handler(*quote_status, line[char_to_check]);
		char_to_check++;
	}
	char_to_check--;
	word = ft_substr(line, word_start, char_to_check - word_start + 1);
	if (!word)
		return (ERROR_HARD);
	if (!tokenlist_add(list_start, WORD, word, quote_save))
		return (ERROR_HARD);
	return (char_to_check);
}

int	everything_except_word_handler(t_token *list_start, char *line, int *i)
{
	int	type;

	type = START;
	if (line[*i] == '|')
		type = PIPE;
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		type = HEREDOC;
		(*i) += 1;
	}
	else if (line[*i] == '<')
		type = REDIR_IN;
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		type = REDIR_APPEND;
		(*i) += 1;
	}
	else if (line[*i] == '>')
		type = REDIR_OUT;
	if (!tokenlist_add(list_start, type, NULL, DEFAULT_Q))
		return (ERROR_HARD);
	return (RET_OK);
}

int	tokenizer_loop(char *line, t_token *list_start, int *quote_status)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		*quote_status = quote_handler(*quote_status, line[i]);
		if (ft_isspace(line[i]))
			continue ;
		if (not_a_word(line[i], line[i + 1]))
		{
			if (everything_except_word_handler(list_start, line,
					&i) == ERROR_HARD)
				return (ERROR_HARD);
		}
		else
		{
			i = word_handler(i, line, list_start, quote_status);
			if (i == ERROR_HARD)
				return (ERROR_HARD);
		}
	}
	return (RET_OK);
}

/* we want to structure the input and save it in a linked list. in order to
 * know what elements occur, we have to do certain checks to the string.
 * these tokens are possible:
 *   - PIPE: |
 *   - HEREDOC: <<
 *   - REDIR_IN: <
 *   - REDIR_APPEND: >>
 *   - REDIR_OUT: >
 *   - WORD
 *   additionally we have to save the state of the quotes (default, in single,
 *	in double quotes)
 */
t_token	*tokenizer(char *line)
{
	t_token	*list_start;
	int		quote_status;

	if (!line)
		return (NULL);
	list_start = ft_calloc(1, sizeof(t_token));
	if (!list_start)
		return (NULL);
	init_token(list_start);
	quote_status = DEFAULT_Q;
	if (tokenizer_loop(line, list_start, &quote_status) == ERROR_HARD)
	{
		cleanup_token_list(list_start);
		return (NULL);
	}
	return (list_start);
}

// debug stuff, remove before finalizing
/* void	print_tokens(t_token *start)
{
	int	i;

	i = 0;
	start = start->next;
	while (start)
	{
		printf("Token %d: Type %d, ", ++i, start->type);
		if (start->type == WORD || start->type == WORD_AFTER_HEREDOC)
			printf(" value: %s, quote_status: %d", start->str,
				start->quote_status);
		printf("\n");
		start = start->next;
	}
}

void	print_token_type(int type)
{
	if (type == PIPE)
		printf("PIPE ");
	else if (type == HEREDOC)
		printf("HEREDOC ");
	else if (type == REDIR_IN)
		printf("REDIR_IN ");
	else if (type == REDIR_APPEND)
		printf("REDIR_APPEND ");
	else if (type == REDIR_OUT)
		printf("REDIR_OUT ");
}
*/