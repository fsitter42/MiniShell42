/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:52:01 by slambert          #+#    #+#             */
/*   Updated: 2026/03/23 13:03:55 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_last_exit_code;


int f_is_syntax_valid(t_data *data);
/*
	input parsing - Stefan
	TODO:
	- prompt blinking cursor?
	- detection of syntax errors
	- creation of pipes and correct redirection of pipe ends
	execution - Frido
	TODO:
	- handle environment variables (how do we store them?)
	- find the PATHS for the given commands (absolute/relative paths also have to work)
	- do the actual execve
	- echo with option -n
	- cd with only a relative or absolute path
	- pwd with no options
	- export with no options
	- unset with no options
	- env with no options or arguments
	-
	TODO - not defined:
	- handle exit status (stored in $?)
	- ctrl-C displays a new prompt on a new line
	- ctrl-D exits the shell
	- ctrl-\ does nothing
*/

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

int	is_token_list_empty(t_token *token_list)
{
	if (!token_list || !token_list->next)
	{
		printf("Token list is empty\n");
		return (1);
	}
	return (0);
}

// changes the type of the WORD token after the HEREDOC to WORD_AFTER_HEREDOC
// returns 1 on error and 0 on success
int	handle_delimiter(t_token *token_list)
{
	while (1)
	{
		while (token_list && token_list->type != HEREDOC)
			token_list = token_list->next;
		if (!token_list)
			return (0);
		// heredoc found
		if (token_list->next && token_list->next->type == WORD)
		{
			token_list->next->type = WORD_AFTER_HEREDOC;
			token_list = token_list->next;
		}
		else
			return (1);
		// token_list = token_list->next;
	}
	return (0);
}

// this function does everything that is needed that ONE LINE is being executed correctly
int	handle_single_line(char *line, char **envp, t_data *data)
{
	int ret;
	t_token	*token_list;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	if (ft_strncmp(line, "exit", 5) == 0)
	{
		sfbf_free_all(data);
		my_exit_function(NULL);
	}
	//printf("%s is going to be tokenized\n", line);
	token_list = tokenizer(line);
	if (!token_list)
		return (1);
	if (handle_delimiter(token_list) == 1)
		return (cleanup_token_list(token_list), 1);
	//printf("\nBEFORE EXPANSION\n");
	//print_tokens(token_list);
	if (expansion(token_list, envp, data) == 1)
		return (cleanup_token_list(token_list), 1);
	//printf("\nAFTER EXPANSION\n");
	//print_tokens(token_list);
	if (word_split(token_list) == 1)
		return (cleanup_token_list(token_list), 1);
	//printf("\nAFTER WORD SPLIT\n");
	//print_tokens(token_list);
	if (!is_token_list_empty(token_list))
	{
    	cmd_list = create_command_list(token_list->next);
    	if (!cmd_list)
    	    return (cleanup_token_list(token_list), 1);	
    	cleanup_token_list(token_list);
    	data->cmds = cmd_list; 	
    	if (f_is_syntax_valid(data) == 0)
    	{
        	cleanup_command_list(cmd_list);
        	data->cmds = NULL;
        	return (1); 
    	}
    	ret = eggsecute(data); //frido geändert
	}
	else
		cleanup_token_list(token_list);
	//sfbf_free_all(data);
	cleanup_command_list(cmd_list);
	data->cmds = NULL;
	return (ret);
}

/* this is the default mode in where the users enters stuff
 */
void	normal_mode(int argc, char **argv, char **envp, t_data *data)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		// printf("read line is %s\n", line);
		if (!line)
			break ;
		if (ft_strncmp(line, "", 1) == 0)
		{
			free(line);
			continue ;
		}
		if (*line)
			add_history((const char *)line);
		if (handle_single_line(line, envp, data) == 1)
			printf("handle_single_line failed\n"); // TODO brauch ma??
		free(line);
	}
}

static void	cleanup_split_result(char **strs, int start)
{
	while (strs[start])
	{
		free(strs[start]);
		start++;
	}
	free(strs);
}

/* the difference here is that we have one string that is given to
 *  minishell, no interactions with the user. all lines have to be separated
 *  by a semicolon (;) and will be executed one afprintf("hello nur\n");ter each other.

	*  note: if we want to use a $ sign (var) we have to explicitly escape that character (\\ before §).

	*  otherwise the shell tries to expand the variable before it is given to minishell as an input
 */
void	debug_mode(char *input, char **envp, t_data *data)
{
	char	**strs;
	int		i;

	strs = NULL;
	strs = ft_split(input, ';');
	if (!strs)
		my_exit_function("ft_split returned NULL");
	i = -1;
	while (strs[++i])
	{
		if (handle_single_line(strs[i], envp, data) == 1)
		{
			cleanup_split_result(strs, i);
			my_exit_function("handle_single_line failed\n");
		}
		data->cmds = NULL;
		free(strs[i]);
	}
	free(strs);
}

void	sfbf_free_all(t_data *data)
{
	if (!data)
		return ;
	if (data->env->envp_lst)
		f_free_env_list(data->env->envp_lst); //vll nmoch freen zusätzliuch
	if (data->env->envp_updated)
		f_free_envp(data->env->envp_updated);
	// if (data->env->envp_ori)
	// 	free(data->env->envp_ori);	
	free(data->env);
	cleanup_command_list(data->cmds);
	free(data);
}

t_data	*sfbf_init_all(char **envp)
{
	t_data	*data;
	char **args;
	
	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
		return (NULL);
	data->env = f_init_envp(envp);
	if (!data->env)
		return (free(data), NULL);
	args = (char *[]){"export", "OLDPWD", NULL};
	f_export(data, args);
	return (data);
}

/* we can start minishell in normal (user input) mode (argc = 1) or in
 * debug mode (with the -d flag followed by a string) - argc = 3
 */
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		last_exit_code;

	if (argc != 1 && argc != 3)
		return (printf("wrong syntax - argc not 1 or 3\n"), 1);
	if (argc == 1)
	{
		// hier env init
		data = sfbf_init_all(envp);
		if (!data)
			return (1);
		

		normal_mode(argc, argv, envp, data);
		
	}
	else
	{
		if (ft_strncmp(argv[1], "-d", 2) != 0)
			return (printf("wrong syntax - did you use the -d flag?\n"), 1);
		data = sfbf_init_all(envp);
		if (!data)
			return (1);
		//f_print_env(data->env->envp_lst);
		debug_mode(argv[2], envp, data);
	}
	sfbf_free_all(data);
}

int f_is_syntax_valid(t_data *data)
{
    t_cmd *tmp;

    tmp = data->cmds;
    while (tmp)
    {
        if (!tmp->cmd && !tmp->redirs)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2);
            data->last_exit_code = 258;
            return (0);
        }
        tmp = tmp->next;
    }
    return (1);
}

/* size_t count_cmds(t_token *cmd_list)
{
	size_t size;

	size = 0;
	while (cmd_list)
	{
		size++;
		cmd_list = cmd_list->next;
	}
	return (size);
} */