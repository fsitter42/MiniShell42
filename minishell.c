/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:48:22 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 12:34:37 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include <stdio.h>

/*
	input parsing - Stefan
	TODO:
	- display a prompt
	- (blinking cursor?)
	- detection of syntax errors
	- creation of pipes and correct redirection of pipe ends
	- backslash / semicolons handling
	- handle single/double quotes
	- redirection input/output (</>)
	- delimiter (<</>>):
	- handle environment variables (how do we store them?)
	execution - Frido
	TODO:
	- find the PATHS for the given commands (absolute/relative paths also have to work)
	- do the actual execve
	- echo with option -n
	- cd with only a relative or absolute path
	- pwd with no options
	- export with no options
	- unset with no options
	- env with no options or arguments
	- exit with no options
	-
	TODO - not defined:
	- history
	- handle exit status (stored in $?)
	- ctrl-C displays a new prompt on a new line
	- ctrl-D exits the shell
	- ctrl-\ does nothing
	-
	*/

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
int	loop(char *line)
{
	t_token	*cur_elem;

	// 1. tokenizer
	printf("'%s' is going to be tokenized\n", line);
	cur_elem = NULL;
	cur_elem = tokenizer(line);
	free(line);
}

void	init_token(t_token *token)
{
	token->type = START;
	token->str = NULL;
	token->status = WIP;
	token->next = NULL;
}

void	tokenlist_add(t_token *list_start, int type, char *str)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	// if (!new_token)
	// error handling
	while (list_start->next)
		list_start = list_start->next;
	list_start->next = new_token;
	init_token(new_token);
	new_token->type = type;
	new_token->str = str;
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
	int		word_start;
	char	*word = NULL;

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
		// WORD
		word_start = i;
		while (is_part_of_word(line[i]))
			i++;
		i--;
		word = ft_strdup(ft_substr(line, word_start, i - word_start + 1));
		//if (!word)
		//error hanlding
		tokenlist_add(list_start, WORD, word);
		printf("WORD ");
	}
	printf("\n");
	return list_start;
}

int	is_part_of_word(char c)
{
	if (c == '|' || c == '<' || c == '>' || isspace(c) || c == '\0')
		return (0);
	return (1);
}

/* this is the default mode in where the users enters stuff
 */
int	normal_mode(int argc, char **argv, char **envp)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			return (-1);
		if (!strcmp(line, ""))
			add_history((const char *)line);
		loop(line);
	}
}

/* the difference here is that we have one string that is given to
 *  minishell, no interactions with the user. all lines have to be separated
 *  by a semicolon (;) and will be executed one after each other.
 */
int	debug_mode(char *input, char **envp)
{
	char	**strs;
	int		i;

	strs = NULL;
	strs = ft_split(input, ';');
	if (!strs)
		exit(EXIT_FAILURE);
	i = -1;
	while (strs[++i])
		loop(strs[i]);
}

/* we can start minishell in normal (user input) mode (argc = 1) or in
 * debug mode (with the -d flag followed by a string) - argc = 3
 */
int	main(int argc, char **argv, char **envp)
{
	// if the program was compiled with the -d flag and argc is 2
	// we run minishell in debug mode, otherwise normal mode
	if (argc != 1 && argc != 3)
		return (printf("wrong syntax - argc not 1 or 3\n"), 1);
	if (argc == 1)
		normal_mode(argc, argv, envp);
	else
	{
		if (ft_strncmp(argv[1], "-d", 2) != 0)
			return (printf("wrong syntax - did you use the -d flag?\n"), 1);
		debug_mode(argv[2], envp);
	}
}
