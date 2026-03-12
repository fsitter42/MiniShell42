/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/03/12 12:07:19 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_part_of_word(char c, int *quote_status);

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->quote_status = DEFAULT_QUOTE;
	token->consume_status = UNCONSUMED;
	token->next = NULL;
}

t_token	*tokenlist_add(t_token *list_start, int type, char *str,
		int quote_status)
{
	t_token	*new_token;
	t_token	*current;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (free(str), NULL);
	current = list_start;
	while (current->next)
		current = current->next;
	current->next = new_token;
	init_token(new_token);
	new_token->type = type;
	new_token->str = str;
	new_token->quote_status = quote_status;
	return (new_token);
}

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
	printf("WORD ");
	return (char_to_check);
}

// this is for debugging only, will be removed!
void	print_tokens(t_token *start)
{
	int	i;

	i = 0;
	start = start->next;
	while (start)
	{
		printf("Token %d: Type %d, ", ++i, start->type);
		if (start->type == WORD || start->type == VAR
			|| start->type == WORD_AFTER_HEREDOC)
			printf(" value: %s, quote_status: %d", start->str,
				start->quote_status);
		printf("\n");
		start = start->next;
	}
}

int	is_quote(char c)
{
	if (c == '\"' || c == '\'')
		return (1);
	return (0);
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

//for debugging
void print_token_type(int type)
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

/* int	everything_except_word_handler(t_token *list_start, char *line, int *i)
{
	if (line[*i] == '|')
	{
		printf("PIPE ");
		if (!tokenlist_add(list_start, PIPE, NULL, DEFAULT_QUOTE))
			return (1);
		return 0;
	}
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		printf("HEREDOC ");
		if (!tokenlist_add(list_start, HEREDOC, NULL, DEFAULT_QUOTE))
			return (1);
		(*i) += 1;
		return 0;
	}
	if (line[*i] == '<')
	{
		printf("REDIR_IN ");
		if (!tokenlist_add(list_start, REDIR_IN, NULL, DEFAULT_QUOTE))
			return (1);
		return 0;
	}
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		printf("REDIR_APPEND ");
		if (!tokenlist_add(list_start, REDIR_APPEND, NULL, DEFAULT_QUOTE))
			return (1);
		(*i) += 1;
		return 0;
	}
	if (line[*i] == '>')
	{
		printf("REDIR_OUT ");
		if (!tokenlist_add(list_start, REDIR_OUT, NULL, DEFAULT_QUOTE))
			return (1);
	}
	return 0;
} */

int	everything_except_word_handler(t_token *list_start, char *line, int *i)
{
	int type;
	
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
	print_token_type(type);
	if (!tokenlist_add(list_start, type, NULL, DEFAULT_QUOTE))
		return (1);
	return 0;
}

//returns 1 if not a word
int not_a_word(char c1, char c2)
{
	if (c1 == '|')
		return 1;
	if (c1 == '<' && c2 == '<')
		return 1;
	if (c1 == '<')
		return 1;
	if (c1 == '>' && c2 == '>')
		return 1;
	if (c1 == '>')
		return 1;
	return 0;
}

/* we want to structure the input and save it in a linked list. in order to know what elements
 *   occur,
	we have to do certain checks to the string. these tokens are possible:
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
	int		i;
	int		quote_status;

	// TODO difference between these 2 cases
	if (!line || ft_strncmp(line, "", 1) == 0)
		return (NULL);
	if (quote_sytanx_check(line))
	{
		printf("quote syntax check failed\n");
		return (NULL);
	}
	list_start = ft_calloc(1, sizeof(t_token));
	if (!list_start)
	{
		free(line);
		my_exit_function("malloc fail");
	}
	init_token(list_start);
	quote_status = DEFAULT_QUOTE;
	i = -1;
	while (line[++i])
	{
		quote_status = quote_handler(quote_status, line[i]);
		if (isspace(line[i]))
			continue ;
		if (not_a_word(line[i], line[i + 1]))
		{
			if (everything_except_word_handler(list_start, line, &i) == 1)
				return (NULL);
		}
		else
		{
			i = word_and_var_handler(i, line, list_start, &quote_status);
			if (i == -1)
				return (NULL);
		}
	}
	// printf("\nBEFORE EXPANSION\n");
	// print_tokens(list_start);
	return (list_start);
}

int	is_part_of_word(char c, int *quote_status)
{
	if (*quote_status == DEFAULT_QUOTE && (c == '|' || c == '<' || c == '>'
			|| isspace(c) || c == '\0'))
		return (0);
	return (1);
}
