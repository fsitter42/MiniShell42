/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:48:22 by slambert          #+#    #+#             */
/*   Updated: 2026/02/10 11:06:05 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int main (int argc, char **argv, char **envp)
{
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

    //general loop
    while (1)
    {
/*      1. wait for input
        2. input parsing
        3. execution
        4. output
        5. repeat 
*/
    }
}
