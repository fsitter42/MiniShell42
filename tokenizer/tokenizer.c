/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:06 by slambert          #+#    #+#             */
/*   Updated: 2026/02/24 12:30:14 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_part_of_word(char c);

/* this is where the magic happens, same in debug and in normal mode
	tokenizer(line);
	syntax check , if negative return error msg and return prompt
	if positive, continue
	create list of structs that are relevant for execution.
	this includes:
	- expansion
	- quotes
	- list of cmds
	free(line);
	pass this list to execute part of program (frido)
*/
// this function does everything that is needed that ONE LINE is being executed correctly
void	handle_single_line(char *line)
{
	t_token	*token_list;
	int should_exit;

	should_exit = 0;
	if (ft_strncmp(line, "exit", 4) == 0)
		should_exit = 1;
	// 1. tokenizer
	printf("'%s' is going to be tokenized\n", line);
	token_list = tokenizer(line);
	//do something with token_list
	free(line);
	create_command_list(token_list);
	cleanup_token_list(token_list);
	if (should_exit)
		my_exit_function();
}

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->status = STATUS_UNSET;
	token->next = NULL;
}

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
	//free (word);
	return (i);
}

//this is for debugging only, will be removed!
void print_tokens(t_token *start)
{
	int i = 0;
	
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

/* we want to structure the input and save it in a linked list. in order to know what elements
   occur, we have to do certain checks to the string. these tokens are possible:
   - PIPE: |
   - HEREDOC: <<
   - REDIR_IN: <
   - REDIR_APPEND: >>
   - REDIR_OUT: >
   - WORD
   - END
   return: NULL on empty line or a pointer to the first element
*/
t_token	*tokenizer(char *line)
{
	t_token	*list_start;
	int		i;

	// TODO difference between these 2 cases
	if (!line || ft_strncmp(line, "", 1) == 0)
		return (NULL);
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
