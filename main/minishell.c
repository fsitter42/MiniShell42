/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:52:01 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 16:59:35 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
*/

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
		if (*line)
			add_history((const char *)line);
		handle_single_line(line);
	}
}

/* the difference here is that we have one string that is given to
 *  minishell, no interactions with the user. all lines have to be separated
 *  by a semicolon (;) and will be executed one after each other.

	*  note: if we want to use a $ sign (var) we have to explicitly escape that character (\\ before §).

	*  otherwise the shell tries to expand the variable before it is given to minishell as an input
 */
int	debug_mode(char *input, char **envp)
{
	char	**strs;		//strategy to free this on error: we have to get that pointer somehow in the 
						//cleanup function, so i assume we have to store it somewhere in the linked list
						//BUT: this function is only for testing, so actually we don't have to care
	int		i;

	strs = NULL;
	strs = ft_split(input, ';');
	if (!strs)
		my_exit_function();
	i = -1;
	while (strs[++i])
		handle_single_line(strs[i]);
	free(strs);	//this is memory safe because the actual lines are freed in handle_single_line
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
