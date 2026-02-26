/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/02/26 15:54:01 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_part_of_word(char c);

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->status = STATUS_UNSET;
	token->consume_status = UNCONSUMED;
	token->quote_status = DEFAULT;
	token->next = NULL;
}

// TODO eventuell ohne malloc, einfach normale variable
void	tokenlist_add(t_token *list_start, int type, char *str)
{
	t_token	*new_token;

	if (list_start->status == STATUS_SET)
	{
		new_token = ft_calloc(1, sizeof(t_token));
		// if (!new_token)
		// error handling
		while (list_start->next)
			list_start = list_start->next;
		list_start->next = new_token;
		init_token(new_token);
	}
	else
		new_token = list_start;
	new_token->type = type;
	new_token->str = str;
	// new_token->quote_status = quote_status;
	new_token->status = STATUS_SET;
}

/* 2 modes: WORD mode just handles a normal word. VAR mode is executed if a $ is found and starts form i+1
 */
int	word_and_var_handler(int i, char *line, t_token *list_start, int mode)
{
	int		word_start;
	char	*word;
	char	*temp;

	word_start = i;
	while (is_part_of_word(line[i]))
		i++;
	i--;
	temp = ft_substr(line, word_start, i - word_start + 1);
	word = ft_strdup(temp);
	free(temp);
	// if (!word)
	// error hanlding
	if (mode == MODE_WORD)
	{
		tokenlist_add(list_start, WORD, word);
		printf("WORD ");
	}
	else if (mode == MODE_VAR)
	{
		tokenlist_add(list_start, VAR, word);
		printf("VAR ");
	}
	// free (word);
	return (i);
}

// this is for debugging only, will be removed!
void	print_tokens(t_token *start)
{
	int	i;

	i = 0;
	while (start)
	{
		printf("Token %d: %d, ", ++i, start->type);
		if (start->type == WORD || start->type == VAR)
			printf(" value: %s\n", start->str);
		else
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

/* quote handling logic:
*  step 1: 	quote syntax counter: counts if every opening quote has a
*			dedicated closing quote of the same type. if not: display error
			(do i actually need that step or could i include it in step2?)
*  step 2:	in the tokenizer loop through the line and if " or ' is encountered,
*			set the tokens the the specific quote_status until the quote is closed.
+
*			what about "'" '"' $bla '"' "'"
*
*/
int	quote_handler(int quote_status, char c)
{
}

/* we want to structure the input and save it in a linked list. in order to know what elements
   occur, we have to do certain checks to the string. these tokens are possible:
   - PIPE: |
   - HEREDOC: <<
   - REDIR_IN: <
   - REDIR_APPEND: >>
   - REDIR_OUT: >
   - WORD
   - END
   additionally we have to save the state of the quotes (default, in single,
	in double quotes)
   return: NULL on empty line or a pointer to the first element
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
		return NULL;
	}
		
	list_start = ft_calloc(1, sizeof(t_token));
	// if (!list_start)
	// error handling
	init_token(list_start);
	i = -1;
	//quote_status = DEFAULT;
	// TODO we somehow need to store the information on quotes here.
	// on each character that is either " or ' we have to save the
	// info for all tokens regarding quote. additionally we have
	// to check if the quotes are closed. otherwise, syntax error
	// only after that is done we can think about variable expansion
	while (line[++i])
	{
		// if (is_quote(line[i]))
		//	quote_status = quote_handler[quote_status, line[i]];
		if (isspace(line[i]))
			continue ;
		if (line[i] == '|')
		{
			printf("PIPE ");
			tokenlist_add(list_start, PIPE, NULL);
			continue ;
		}
		if (line[i] == '<' && line[i + 1] == '<')
		{
			printf("HEREDOC ");
			tokenlist_add(list_start, HEREDOC, NULL);
			i++;
			continue ;
		}
		if (line[i] == '<')
		{
			printf("REDIR_IN ");
			tokenlist_add(list_start, REDIR_IN, NULL);
			continue ;
		}
		if (line[i] == '>' && line[i + 1] == '>')
		{
			printf("REDIR_APPEND ");
			tokenlist_add(list_start, REDIR_APPEND, NULL);
			i++;
			continue ;
		}
		if (line[i] == '>')
		{
			printf("REDIR_OUT ");
			tokenlist_add(list_start, REDIR_OUT, NULL);
			continue ;
		}
		if (line[i] == '$')
		{
			i++;
			i = word_and_var_handler(i, line, list_start, MODE_VAR);
			continue ;
		}
		// WORD
		i = word_and_var_handler(i, line, list_start, MODE_WORD);
	}
	printf("\n");
	print_tokens(list_start);
	return (list_start);
}

int	is_part_of_word(char c)
{
	if (c == '|' || c == '<' || c == '>' || isspace(c) || c == '\0')
		return (0);
	return (1);
}
