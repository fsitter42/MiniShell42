/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 13:29:16 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "./libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

typedef struct s_token
{
	int type; // enum
	char			*str;
	int				status;
	struct s_token	*next;
	// struct s_token	*previous;

}					t_token;

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
enum				word_mode
{
	MODE_WORD = 0,
	MODE_VAR
};

enum				e_token_status
{
	STATUS_UNSET = 0,
	STATUS_SET
};

t_token				*tokenizer(char *line);