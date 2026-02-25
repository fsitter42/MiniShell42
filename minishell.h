/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/02/25 15:37:31 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "./libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

typedef struct s_token
{
	int type; // enum e_word_mode
	char			*str;
	int status; // enum e_token_status
	int consume_status;//enum e_consume_status
	struct s_token	*next;
}					t_token;


//TODO pipes & fds
typedef struct s_cmd
{
	char	*cmd;
	int		is_builtin;
	char	**args;
	char	*path;
	char	*infile;
	int		in_fd;
	char	*outfile;
	int		out_fd;
	int		append;
	struct s_cmd *next;	//if the command has a next one, we need a pipe
}					t_cmd;

// enums
enum				e_token_types
{
	START = 0,
	WORD,
	VAR,
	PIPE,
	HEREDOC, 		//<<
	REDIR_IN,		//<
	REDIR_APPEND,	//>>
	REDIR_OUT,		//>
};

// is used in tokenizer, word_and_var_handler
enum				e_word_mode
{
	MODE_WORD = 0,
	MODE_VAR
};

//used in AST, flagging tokens
enum				e_consume_status
{
	UNCONSUMED = 0,
	CONSUMED
};

//TODO this is currently used to use a different strategy to add a token to the
//list if it is the first element. should be changed to the logic like in the 
//command list later on
enum				e_token_status
{
	STATUS_UNSET = 0,
	STATUS_SET
};

enum				e_bool
{
	FALSE = 0,
	TRUE
};

//main


//AST
t_cmd* create_command_list(t_token *token_list);

//cleanup
void my_exit_function();
void cleanup_token_list(t_token *token_list);
//tokenizer
void	handle_single_line(char *line);
t_token	*tokenizer(char *line);

