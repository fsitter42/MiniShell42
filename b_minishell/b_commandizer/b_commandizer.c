/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_commandizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:57 by slambert          #+#    #+#             */
/*   Updated: 2026/04/19 00:52:40 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*  here we want to create a second linked list that consumes the token linked
 *  list and creates the exact structure that we need in order to be able to
 *  execute commands and create the pipes needed.
 *  we have to find a logic that extracts the correct amount of words that
 *  form one command (each flag
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

static int	handle_heredoc_redir(t_token **tl, t_cmd *cmd)
{
	t_redir	*cur;
	t_redir	*latest_heredoc;

	if (add_r_t_c(cmd, HEREDOC, NULL, (*tl)->next->str) == ERROR_HARD)
		return (ERROR_HARD);
	cur = cmd->redirs;
	latest_heredoc = NULL;
	while (cur)
	{
		if (cur->type == HEREDOC
			&& (!latest_heredoc || cur->id > latest_heredoc->id))
			latest_heredoc = cur;
		cur = cur->next;
	}
	if (latest_heredoc && (*tl)->next->quote_status != DEFAULT_Q)
		latest_heredoc->delimiter_is_quoted = TRUE;
	return (RET_OK);
}

int	handle_redirection(t_token **tl, t_cmd *cmd)
{
	int	ret;

	if (!is_valid_redir_t(*tl))
		return (ERROR_SOFT);
	ret = RET_OK;
	if ((*tl)->type == REDIR_IN)
		ret = add_r_t_c(cmd, REDIR_IN, (*tl)->next->str, NULL);
	else if ((*tl)->type == REDIR_OUT)
		ret = add_r_t_c(cmd, REDIR_OUT, (*tl)->next->str, NULL);
	else if ((*tl)->type == HEREDOC)
		ret = handle_heredoc_redir(tl, cmd);
	else if ((*tl)->type == REDIR_APPEND)
		ret = add_r_t_c(cmd, REDIR_APPEND, (*tl)->next->str, NULL);
	if (ret == ERROR_HARD)
		return (ERROR_HARD);
	return (shift_and_consume_tl(tl, 2), RET_OK);
}

int	handle_word(t_token **token_list, t_cmd *cmd)
{
	int		i;

	i = 0;
	if (cmd->cmd == NULL)
	{
		cmd->cmd = ft_strdup((*token_list)->str);
		if (!cmd->cmd)
			return (ERROR_HARD);
	}
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup((*token_list)->str);
	if (!(cmd->args[i]))
		return (ERROR_HARD);
	shift_and_consume_tl(token_list, 1);
	return (RET_OK);
}

int	create_single_cmd(t_token *token_list, int size, t_cmd **cmd)
{
	int	ret;

	*cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!*cmd)
		return (ERROR_HARD);
	init_cmd(*cmd);
	if (size == 0)
		(*cmd)->is_redir_only_cmd = 1;
	if (init_args_array(*cmd, size) == ERROR_HARD)
		return (free(*cmd), ERROR_HARD);
	(*cmd)->args_count = size;
	while (token_list && token_list->type != PIPE)
	{
		if (is_token_type_redirection(token_list))
		{
			ret = handle_redirection(&token_list, *cmd);
			if (ret != RET_OK)
				return (cleanup_command_list(*cmd), ret);
			continue ;
		}
		ret = handle_word(&token_list, *cmd);
		if (ret != RET_OK)
			return (cleanup_command_list(*cmd), ret);
	}
	return (RET_OK);
}

/*
 * Step 1: go through the token list and count how many pipes there are.
 * Step 2: for each command (pipes
	+ 1): call separate function create_single_cmd
 * returns status enum value, frees cmd list itself on error
 */
int	create_command_list(t_token *token_list, t_cmd **cmd_list)
{
	int		pipes_count;
	int		i;
	int		ret;
	t_cmd	*cmd;
	int		size;

	pipes_count = count_pipes(token_list);
	*cmd_list = NULL;
	i = -1;
	while (++i < pipes_count + 1)
	{
		size = count_size_for_args_array(token_list);
		cmd = NULL;
		ret = create_single_cmd(token_list, size, &cmd);
		if (ret != RET_OK)
			return (cleanup_command_list(*cmd_list), ret);
		add_cmd_to_cmd_list(cmd_list, cmd);
		shift_tl_to_next_pipe(&token_list);
		cmd->is_builtin = f_is_builtin(cmd->cmd);
	}
	return (RET_OK);
}

// brauch ma nima
/* int	heredoc_handler(t_cmd *cmd, t_token *token_list)
{
	int	i;
	int	j;

	if (token_list->next->type == WORD_AFTER_HEREDOC)
	{
		cmd->delimiter = ft_strdup(token_list->next->str);
		if (!cmd->delimiter)
			return (ERROR_HARD);
	}
	else
		return (ERROR_SOFT);
	return (RET_OK);
} */
/* void	print_command_list(t_cmd *start)
{
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

void	print_args_array(char **args)
{
	int	i;

	i = -1;
	printf("args: ");
	while (args[++i])
		printf("\"%s\" ", args[i]);
}
*/
