/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/03/02 20:46:36 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_part_of_word(char c);

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->consume_status = UNCONSUMED;
	token->next = NULL;
}

t_token	*tokenlist_add(t_token *list_start, int type, char *str)
{
	t_token	*new_token;
	t_token	*current;

	new_token = ft_calloc(1, sizeof(t_token));
	// if (!new_token)
	// error handling
	current = list_start;
	while (current->next)
		current = current->next;
	current->next = new_token;
	init_token(new_token);
	new_token->type = type;
	new_token->str = str;
	return (new_token);
}

int	word_and_var_handler(int i, char *line, t_token *list_start)
{
	int		word_start;
	int		char_to_check;
	char	*word;

	word_start = i;
	char_to_check = word_start + 1;

	while (TRUE)
	{
		if (!is_part_of_word(line[char_to_check]))
			break;
		char_to_check++;
	}
	char_to_check--;
	word = ft_substr(line, word_start, char_to_check - word_start + 1);
	// if (!word)
	// error hanlding
	tokenlist_add(list_start, WORD, word);
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
		if (start->type == WORD || start->type == VAR)
			printf(" value: %s", start->str);
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
		else if ((cur_status == 1 && line[i] == '\'') || (cur_status == 2 && line[i] == '\"'))
			cur_status = 0;
	}
	return cur_status;
}

/* we want to structure the input and save it in a linked list. in order to know what elements
   occur, we have to do certain checks to the string. these tokens are possible:
   - PIPE: |
   - HEREDOC: <<
   - REDIR_IN: <
   - REDIR_APPEND: >>
   - REDIR_OUT: >
   - WORD
   additionally we have to save the state of the quotes (default, in single,
	in double quotes)
   return: NULL on empty line or a pointer to the first element
   TODO: fix bug where it segfaults when < or > is in the end
*/
t_token	*tokenizer(char *line)
{
	t_token	*list_start;
	int		i;
	
	// TODO difference between these 2 cases
	if (!line || ft_strncmp(line, "", 1) == 0)
		return (NULL);
	if (quote_sytanx_check(line))
	{
		printf("quote syntax check failed\n");
		return NULL;
	}
	list_start = ft_calloc(1, sizeof(t_token));
	// if (!list_start)
	// error handling
	init_token(list_start);
	i = -1;
	while (line[++i])
	{
		if (isspace(line[i]))
			continue ;
		if (line[i] == '|')
		{
			//printf("PIPE ");
			tokenlist_add(list_start, PIPE, NULL);
			continue ;
		}
		if (line[i] == '<' && line[i + 1] == '<')
		{
			//printf("HEREDOC ");
			tokenlist_add(list_start, HEREDOC, NULL);
			i++;
			continue ;
		}
		if (line[i] == '<')
		{
			//printf("REDIR_IN ");
			tokenlist_add(list_start, REDIR_IN, NULL);
			continue ;
		}
		if (line[i] == '>' && line[i + 1] == '>')
		{
			//printf("REDIR_APPEND ");
			tokenlist_add(list_start, REDIR_APPEND, NULL);
			i++;
			continue ;
		}
		if (line[i] == '>')
		{
			//printf("REDIR_OUT ");
			tokenlist_add(list_start, REDIR_OUT, NULL);
			continue ;
		}
		i = word_and_var_handler(i, line, list_start);
	}
	printf("\nBEFORE EXPANSION\n");
	print_tokens(list_start);
	return (list_start);
}

int	is_part_of_word(char c)
{
	if (c == '|' || c == '<' || c == '>' || isspace(c) || c == '\0')
		return (0);
	return (1);
}
