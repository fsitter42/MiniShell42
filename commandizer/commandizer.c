/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:57 by slambert          #+#    #+#             */
/*   Updated: 2026/03/04 10:47:33 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* here we want to create a second linked list that consumes the token linked list and creates the exact

	*  structure that we need in order to be able to execute the commands and create the pipes needed.

	*  we have to find a logic that extracts the correct amount of words that form one command (each flag
 *  is a new word).
 *  furthermore, the pipes, <,
	> etc. define the redirection of the pipes needed.
 *  example: the input "< file1.txt cat -e | wc
	-l > file2.txt" has the following tokens:
 *  (the numbers are the e_token_types enums,
	value only printed for types VAR and WORD)
 *  Token 1: 5,
 *  Token 2: 1,  value: file1.txt
 *  Token 3: 1,  value: cat
 *  Token 4: 1,  value: -e
 *  Token 5: 3,
 *  Token 6: 1,  value: wc
 *  Token 7: 1,  value: -l
 *  Token 8: 7,
 *  Token 9: 1,  value: file2.txt
 *
 *  these tokens have to be translated in the following commands:
 *  cmd 1: cat, args: -e, fd_in: file1.txt, fd_out: pipe1 in
 *  cmd 2: wc, args: -l, fd_in: pipe1 out, fd_out: fd file2.txt
 *
*/

void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->is_builtin = -1;
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->in_fd = -1;
	cmd->outfile = NULL;
	cmd->out_fd = -1;
	cmd->next = NULL;
	cmd->append = FALSE;
}

/* int	return_distance_to_next_pipe(t_token *token_list)
{
	int	i;

	i = 0;
	while (token_list && token_list->type != PIPE)
	{
		i++;
		token_list = token_list->next;
	}
	return (i);
} */

void	shift_and_consume_token_list_by_x(t_token **token, int x)
{
	int	i;

	i = -1;
	while (++i < x)
	{
		(*token)->consume_status = CONSUMED;
		(*token) = (*token)->next;
	}
}

int	is_token_type_redirection(t_token *token)
{
	int	x;

	x = token->type;
	if (x == HEREDOC || x == REDIR_IN || x == REDIR_APPEND || x == REDIR_OUT)
		return (1);
	return (0);
}
/*
*	we can't directly free stuff here bc we have the cleanup_cmd_list
*	(we would free twice if we did that)
*	should be mem safe on error
*/
int	handle_redirection(t_token **token_list, t_cmd *cmd)
{
	if ((*token_list)->type == REDIR_IN)
	{
		cmd->infile = ft_strdup((*token_list)->next->str);
		if (!cmd->infile)
			return 1;
	}
	else if ((*token_list)->type == REDIR_OUT)
	{
		cmd->outfile = ft_strdup((*token_list)->next->str);
		if (!cmd->outfile)
			return 1;
	}
	else if ((*token_list)->type == HEREDOC)
	{
		// HEREDOC STUFF
	}
	else if ((*token_list)->type == REDIR_APPEND)
	{
		cmd->outfile = ft_strdup((*token_list)->next->str);
		if (!cmd->outfile)
			return 1;
		cmd->append = TRUE;
	}
	shift_and_consume_token_list_by_x(token_list, 2);
}

/*
*	returns 1 on error
*/
int	handle_word(t_token **token_list, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->cmd == NULL)
	{
		cmd->cmd = ft_strdup((*token_list)->str);
		if (!cmd->cmd)
			return 1;
	}
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup((*token_list)->str);
	if (!(cmd->args[i]))
		return (1);
	shift_and_consume_token_list_by_x(token_list, 1);
	return (0);
}

int	count_size_for_args_array(t_token *token_list)
{
	int	size;

	size = 0;
	while (token_list && token_list->type != PIPE)
	{
		if (is_token_type_redirection(token_list))
			token_list = token_list->next;
		else
			size++;
		token_list = token_list->next;
	}
	printf("size for args array is %d\n", size);
	return (size);
}

// does the mallocing stuff, returns 0 on success and 1 on error
int	init_args_array(t_cmd *cmd, int size)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(size + 1, sizeof(char *));
	if (!(cmd->args))
		return (1);
	return (0);
}

/* in this function we will have to fill the following members of the
 *  struct t_cmd: cmd (char *), args (char **),
 *  infile / outfile (optional - char *)
 *  walkthrough for the example "< file1.txt cat -e" (REDIT_IN WORD WORD WORD)
 *  if the first token is a REDIR_IN, the next token defines the infile.
 *  the token after that is the cmd itself and all other tokens define args.
 *  if there is a REDIR_OUT encountered, the next token defines the outfile
 *  returns NULL on error
 */
t_cmd	*create_single_cmd(t_token *token_list)
{
	t_cmd	*cmd;
	int		size;

	printf("trying to create a cmd\n");
	cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return NULL;
	init_cmd(cmd);
	size = count_size_for_args_array(token_list);
	if (init_args_array(cmd, size) == 1)
		return (free(cmd), NULL);
	while (token_list && token_list->type != PIPE)
	{
		if (is_token_type_redirection(token_list))
		{
			if (handle_redirection(&token_list, cmd) == 1)
				return NULL;;
			continue ;
		}
		else if (handle_word(&token_list, cmd) == 1)
			return NULL;
	}
	return (cmd);
}

void	add_cmd_to_cmd_list(t_cmd **cmd_list, t_cmd *cmd)
{
	t_cmd	*current;

	if (!(*cmd_list))
	{
		*cmd_list = cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = cmd;
}

void	print_args_array(char **args)
{
	int	i;

	i = -1;
	printf("args: ");
	while (args[++i])
		printf("%s ", args[i]);
}

void	print_command_list(t_cmd *start)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (start)
	{
		printf("Command %d: %s, ", ++i, start->cmd);
		print_args_array(start->args);
		if (start->infile)
			printf("| infile: %s", start->infile);
		if (start->outfile)
			printf("| outfile: %s", start->outfile);
		printf("\n");
		start = start->next;
	}
}

int	count_pipes(t_token *token_list)
{
	int	pipe_count;

	pipe_count = 0;
	while (token_list)
	{
		if (token_list->type == PIPE)
			pipe_count++;
		token_list = token_list->next;
	}
	return (pipe_count);
}

// if we would go out of bounds (null check) the function returns without shifting
void	shift_token_list_to_next_pipe(t_token **token_list)
{
	while (*token_list && (*token_list)->type != PIPE)
		*token_list = (*token_list)->next;
	if (!*token_list)
		return ;
	*token_list = (*token_list)->next;
}

/*
 * Step 1: go through the token list and count how many pipes there are.
 * Step 2: for each command (pipes
	+ 1): call separate function create_single_cmd
 */
t_cmd	*create_command_list(t_token *token_list)
{
	int		pipes_count;
	int		i;
	int		dist_to_next_pipe;
	t_cmd	*cmd_list;
	t_cmd	*cmd;

	pipes_count = count_pipes(token_list);
	cmd_list = NULL;
	i = -1;
	while (++i < pipes_count + 1)
	{
		cmd = create_single_cmd(token_list);
		if (!cmd)
			return (cleanup_command_list(cmd_list), NULL);
		add_cmd_to_cmd_list(&cmd_list, cmd);
		shift_token_list_to_next_pipe(&token_list);
	}
	print_command_list(cmd_list);
	return (cmd_list);
}
