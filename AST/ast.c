/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:53:57 by slambert          #+#    #+#             */
/*   Updated: 2026/02/25 16:04:35 by slambert         ###   ########.fr       */
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

int return_distance_to_next_pipe(t_token *token_list)
{
    int i;

    i = 0;
    while (token_list && token_list->type != PIPE)
    {
        i++;
        token_list = token_list->next;
    }
    return i;
}

//if x would go too far (after the end of the list), the function returns
void shift_token_list_by_x(t_token **token_list, int x)
{
    int i;

    i = -1;
    while (++i < x)
    {
        if (!*token_list)
            return ;
        *token_list = (*token_list)->next;
    }      
}

void shift_and_consume_token_list_by_x (t_token **token, int x)
{
    int i;

    i = -1;
    while (++i < x)
    {
        (*token)->consume_status = CONSUMED;
        (*token) = (*token)->next;
    }
}

int is_token_type_redirection(t_token *token)
{
    int x;
    
    x = token->type;
    if (x == HEREDOC || x == REDIR_IN || x == REDIR_APPEND || x == REDIR_OUT)
        return 1;
    return 0;
}

void handle_redirection(t_token **token_list, t_cmd* cmd)
{
    if ((*token_list)->type == REDIR_IN)
        cmd->infile = (*token_list)->next->str;
    else if ((*token_list)->type == REDIR_OUT)
        cmd->outfile = (*token_list)->next->str;
    else if ((*token_list)->type == HEREDOC)
    {
        //HEREDOC STUFF
    }
    else if ((*token_list)->type == REDIR_APPEND)
    {
        cmd->outfile = (*token_list)->next->str;
        cmd->append = TRUE;
    }
    shift_and_consume_token_list_by_x(token_list, 2);
}

int handle_word(t_token **token_list, t_cmd* cmd)
{
    int i;
    
    i = 0;
    if (cmd->cmd == NULL)
        cmd->cmd = (*token_list)->str;
    //add (*token_list)->str to args
    while (cmd->args[i])
        i++;
    //cmd->args[i] = ft_calloc(ft_strlen((*token_list)->str) + 1, sizeof (char));
    cmd->args[i] = ft_strdup((*token_list)->str);
    if (!(cmd->args[i]))
        return 0;
    shift_and_consume_token_list_by_x(token_list, 1);
    return 1;
}

int count_size_for_args_array(t_token *token_list)
{
    int size;

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
    return size;
}

// does the mallocing stuff, returns 0 on success and -1 on error
int init_args_array(t_cmd *cmd, int size)
{
    int i;

    i = 0;
    cmd->args = ft_calloc(size + 1, sizeof(char*));
    if (!(cmd->args))
        return 0;
    //we can't alloc for the actual strings here because we don't know
    //their lengths here
}

/* in this function we will have to fill the following members of the 
*  struct t_cmd: cmd (char *), args (char **),
*  infile / outfile (optional - char *)
*  walkthrough for the example "< file1.txt cat -e" (REDIT_IN WORD WORD WORD)
*  if the first token is a REDIR_IN, the next token defines the infile.
*  the token after that is the cmd itself and all other tokens define args.
*
*  if there is a REDIR_OUT encountered, the next token defines the outfile
*/
t_cmd *create_single_cmd(t_token *token_list, int no_tokens)
{
    t_cmd *cmd;
    t_token *token_list_copy;
    int size;
    
    token_list_copy = token_list;
    printf("trying to create a cmd with %d tokens\n", no_tokens);
    cmd = ft_calloc(sizeof(t_cmd), 1);
    //if (!cmd)
    //error handling
    init_cmd(cmd);
    size = count_size_for_args_array(token_list);
    if (!init_args_array(cmd, size))
    {
        //error handling
    }
    //NOW WE NEED TO ACTUALLY FILL THE CMD WITH STUFF
    
    while (token_list && token_list->type != PIPE)
    {
        if (is_token_type_redirection(token_list))
        {
            handle_redirection (&token_list, cmd);
            continue;
        }
        else
        {
            if (!handle_word(&token_list, cmd))
            {
                //error handling
            }
        }
                 
    }

    /*
    if (token_list->type == REDIR_IN)
    {
        cmd->infile = token_list->next->str;
        //token_list = token_list->next->next;
        shift_and_consume_token_list_by_x(&token_list, 2);
    }
    if (token_list->type == WORD)
    {
        cmd->cmd = token_list->str;
        //token_list = token_list->next;
        shift_and_consume_token_list_by_x(&token_list, 1);
    }
    //rest ist args aber erst ab [1] [0] ist das gleiche wie cmd
    */
    return cmd;
}

void add_cmd_to_cmd_list(t_cmd **cmd_list, t_cmd *cmd)
{
    if (!(*cmd_list))
    {
        *cmd_list = cmd;
        return;
    } 
    while ((*cmd_list)->next)
        *cmd_list = (*cmd_list)->next;
    (*cmd_list)->next = cmd;
}

void print_args_array(char **args)
{
    int i = -1;

    printf("args: ");
    while (args[++i])
        printf("%s ", args[i]);
}

void print_command_list (t_cmd *start)
{
	int i = 0;
	int j = 0;
    
	while (start)
	{
		printf("Command %d: %s, ", ++i, start->cmd);
		print_args_array(start->args);
        printf("\n");
        start = start->next;
	}
}

/*
 * Step 1: go through the token list and count how many pipes there are.
 * Step 2: for each command (pipes + 1): call separate function create_single_cmd
*/
t_cmd	*create_command_list(t_token *token_list)
{
    t_token *token_list_copy;
    int pipes_count;
    int i;
    int dist_to_next_pipe;
    t_cmd *cmd_list;
    t_cmd *cmd;
    
    token_list_copy = token_list;
    pipes_count = 0;
    while (token_list_copy)
    {
        if (token_list_copy->type == PIPE)
            pipes_count++;
        token_list_copy = token_list_copy->next;
    }
    //now we know that we have pipes_count pipes
    //that means we have pipes_count + 1 cmds
    cmd_list = NULL;
    i = -1;
    while (++i < pipes_count + 1)
    {
        dist_to_next_pipe = return_distance_to_next_pipe(token_list);
        cmd = create_single_cmd(token_list, dist_to_next_pipe);
        add_cmd_to_cmd_list(&cmd_list, cmd);
        shift_token_list_by_x(&token_list, dist_to_next_pipe + 1);
    }
    print_command_list(cmd_list);
    /*
	t_token	*prev_token;
	t_token	*cur_token;
    t_cmd   *cmd;

	cur_token = token_list;
	prev_token = NULL;
	cmd = ft_calloc(1, sizeof(t_cmd));
    if (!cmd)
    //error mgmt
    while (cur_token)
	{
		if (cur_token->type == REDIR_IN && cur_token->next->type == WORD)
            cmd->infile = cur_token->next->str;
        else if (cur_token->type == WORD)
        //else if (cur_token->type == HEREDOC)
        //do heredoc stuff
        
	}*/
}
