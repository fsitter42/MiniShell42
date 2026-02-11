/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:48:22 by slambert          #+#    #+#             */
/*   Updated: 2026/02/11 03:00:12 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

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


/*  we want to split the line into certain tokens.
    possible tokens are:
    - WORD
    - PIPE (|)
    - REDIR_IN (<)
    - REDIR_OUT (>)
    - REDIR_APPEND (>>)
    - HEREDOC (<<)

*/

int is_part_of_word(char c);

void tokenizer (char *line)
{
    int i;

    i = -1;
    while (line[++i])
    {
        if (isspace(line[i]))
            continue;
        if (line[i] == '|')
        {
            printf("PIPE "); 
            continue;
        }
        if (line[i] == '<' && line[i + 1] == '<')
        {
            printf("HEREDOC ");
            i++;
            continue;
        }
        if (line[i] == '<')
        {
            printf("REDIR_IN "); 
            continue;
        }
        if (line[i] == '>' && line[i + 1] == '>')
        {
            printf("REDIR_APPEND ");
            i++;
            continue;
        }
        if (line[i] == '>')
        {
            printf("REDIR_OUT "); 
            continue;
        }
        //WORD
        while (is_part_of_word(line[i]))
            i++;
        i--;
        printf("WORD "); 
    }
    printf("\n");
}

int is_part_of_word(char c)
{
    if (c == '|' || c == '<' || c == '>' || isspace(c))
        return 0;
    return 1;
}
/*  1. wait for input
	2. input parsing
	3. execution
	4. output
	5. repeat
*/
int	main(int argc, char **argv, char **envp)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			return (-1);
        if (!strcmp(line, ""))
            add_history((const char *)line);
        tokenizer(line);
        free (line);
	}
}
