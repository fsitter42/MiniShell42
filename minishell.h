/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/02/23 12:28:10 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

typedef struct s_token
{
	int type; // enum
	char			*str;
	int				status;
	struct s_token	*next;
	//struct s_token	*previous;

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

enum	e_token_status
{
	WIP = 0,
	DONE
};

t_token*				tokenizer(char *line);