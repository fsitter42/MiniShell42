/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 17:01:13 by slambert         ###   ########.fr       */
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
	struct s_token	*next;
	// struct s_token	*previous;

}					t_token;

typedef struct s_cmd
{
	
}					t_cmd;

// enums
enum				e_token_types
{
	START = 0,
	WORD,
	VAR,
	PIPE,
	HEREDOC,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_OUT,
};

// is used in tokenizer, word_and_var_handler
enum				e_word_mode
{
	MODE_WORD = 0,
	MODE_VAR
};

enum				e_token_status
{
	STATUS_UNSET = 0,
	STATUS_SET
};

//main


//AST
void create_command_list(t_token *token_list);

//cleanup
void my_exit_function();
void cleanup_token_list(t_token *token_list);
//tokenizer
int	handle_single_line(char *line);
t_token	*tokenizer(char *line);

