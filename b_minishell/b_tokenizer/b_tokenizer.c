/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_tokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/03/30 16:22:03 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		is_part_of_word(char c, int *quote_status);

// TODO input ""< | > ""6" segfaults
// returns -1 on error
int	word_and_var_handler(int i, char *line, t_token *list_start,
		int *quote_status)
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
		return (-1);
	if (!tokenlist_add(list_start, WORD, word, quote_save))
		return (-1);
	return (char_to_check);
}

/*
 *  checks if every opening quote has a dedicated closing
 *  bracket of the same type
 * returns 0 on success and 1 or 2 on failure
 */
int	quote_sytanx_check(char *line)
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

int	everything_except_word_handler(t_token *list_start, char *line, int *i)
{
	int	type;

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
	if (!tokenlist_add(list_start, type, NULL, DEFAULT_QUOTE))
		return (1);
	return (0);
}

// returns 1 on error
int	tokenizer_loop(char *line, t_token *list_start, int *quote_status)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		*quote_status = quote_handler(*quote_status, line[i]);
		if (isspace(line[i]))
			continue ;
		if (not_a_word(line[i], line[i + 1]))
		{
			if (everything_except_word_handler(list_start, line, &i) == 1)
				return (1);
		}
		else
		{
			i = word_and_var_handler(i, line, list_start, quote_status);
			if (i == -1)
				return (1);
		}
	}
	return (0);
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
 *  	return: NULL on empty line or a pointer to the first element
 *	TODO: fix bug where it segfaults when < or > is in the end
 *
 *	should be mem safe on error
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
	quote_status = DEFAULT_QUOTE;
	if (tokenizer_loop(line, list_start, &quote_status) == 1)
	{
		cleanup_token_list(list_start);
		return (NULL);
	}
	return (list_start);
}

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