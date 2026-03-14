/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/03/14 10:42:36 by slambert         ###   ########.fr       */
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
	int quote_status;
	int consume_status; // enum e_consume_status
	struct s_token	*next;
}					t_token;

/* possible TODOS:
 *  add permission flags for path, infile and outfile
 *
 */
typedef struct s_cmd
{
	char *cmd; // filled by bert
	int				is_builtin;
	char **args; // filled by bert
	char			*path;
	char *infile; // filled by bert
	int				in_fd;
	char *outfile; // filled by bert
	int				out_fd;
	int has_heredoc;
	char *delimiter;
	int append; // filled by bert
	struct s_cmd	*next;
}					t_cmd;

// enums
enum				e_token_types
{
	START = 0,
	WORD,
	VAR,
	PIPE,
	HEREDOC,      //<<
	REDIR_IN,     //<
	REDIR_APPEND, //>>
	REDIR_OUT,    //>
	WORD_AFTER_HEREDOC
};

enum				e_consume_status
{
	UNCONSUMED = 0,
	CONSUMED
};

enum				e_token_status
{
	STATUS_UNSET = 0,
	STATUS_SET
};

enum				e_quote_status
{
	DEFAULT_QUOTE = 0,
	IN_SINGLE_QUOTES,
	IN_DOUBLE_QUOTES
};

enum				e_bool
{
	FALSE = 0,
	TRUE
};

// main

// commandizer
t_cmd				*create_command_list(t_token *token_list);

// cleanup
void 				my_exit_function(char *error_msg);
void				free_token (t_token *token);
void				cleanup_token_list(t_token *token_list);
void				cleanup_command_list(t_cmd *cmd_list);
// tokenizer
void				init_token(t_token *token);
void				handle_single_line(char *line, char **envp);
t_token				*tokenizer(char *line);

// expansion
int					expansion(t_token *list, char **envp);
char				*expand_word_one_pass(char *word);
int					quote_handler(int quote_status, char c);
int					word_split(t_token *list);

//execution
void				eggsecute (t_cmd *cmd_list);

// debug
void				print_tokens(t_token *start);