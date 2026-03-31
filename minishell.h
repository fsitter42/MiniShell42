/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/03/31 23:35:02 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "./b_minishell/libft/libft.h"
# include "f_includes/Libfs/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>

// f

# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define MAX_ENV_LEN 600

typedef struct s_envl	t_envl;
typedef struct s_envp	t_envp;
typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;

// structs
typedef struct s_envl
{
	char				*key;
	char				*val;
	t_envl				*next;

}						t_envl;

typedef struct s_envp
{
	char				**envp_ori;
	t_envl				*envp_lst;
	char				**envp_updated;
}						t_envp;

typedef struct s_redir
{
	char				*file;
	int					type;
	int					id;
	char				*delimiter;
	t_redir				*next;
}						t_redir;

typedef struct s_cmd
{
	char				*cmd;
	int					is_builtin;
	int					is_redir_only_cmd;
	char				**args;
	char				*path;
	t_redir				*redirs;
	int					in_fd;
	int					out_fd;
	t_cmd				*next;
	int					redir_failed;
	int					is_first;
}						t_cmd;

typedef struct s_data
{
	t_cmd				*cmds;
	t_envp				*env;
	int					last_exit_code;
	int					should_exit;
	int					e_has_been_set;
	char				**strs;
}						t_data;

// s

typedef struct s_token
{
	int					type;
	char				*str;
	int					quote_status;
	int					consume_status;
	struct s_token		*next;
}						t_token;

enum					e_token_types
{
	START = 0,
	WORD,
	PIPE,
	HEREDOC,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_OUT,
	WORD_AFTER_HEREDOC
};

enum					e_consume_status
{
	UNCONSUMED = 0,
	CONSUMED
};

enum					e_token_status
{
	STATUS_UNSET = 0,
	STATUS_SET
};

enum					e_quote_status
{
	DEFAULT_Q = 0,
	IN_SINGLE_Q,
	IN_DOUBLE_Q
};

enum					e_bool
{
	FALSE = 0,
	TRUE
};

enum					e_ret_status
{
	RET_OK = 0,
	ERROR_SOFT = -1,
	ERROR_HARD = -2
};

// main
int						line_is_empty(char *line);
int						is_token_list_empty(t_token *token_list);
int						find_delimiters(t_token *token_list);

// init
t_data					*sfbf_init_all(char **envp);

// commandizer
void					init_cmd(t_cmd *cmd);
void					add_cmd_to_cmd_list(t_cmd **cmd_list, t_cmd *cmd);
int						create_command_list(t_token *token_list,
							t_cmd **cmd_list);
int						count_pipes(t_token *token_list);
void					shift_token_list_to_next_pipe(t_token **token_list);
int						count_size_for_args_array(t_token *token_list);
int						init_args_array(t_cmd *cmd, int size);
void					shift_and_consume_token_list_by_x(t_token **token,
							int x);
int						is_token_type_redirection(t_token *token);
int						is_valid_redirection_target(t_token *redir_token);
int						add_r_t_c(t_cmd *cmd, int type, char *str,
							char *delimiter);

// cleanup
void					free_token(t_token *token);
void					cleanup_token_list(t_token *token_list);
void					cleanup_command_list(t_cmd *cmd_list);
void					cleanup_t_data_list(t_data *data);
void					cleanup_split_result(char **strs, int start);
void					sfbf_free_all(t_data *data);

// tokenizer
void					init_token(t_token *token);
int						is_part_of_word(char c, int *quote_status);
int						quote_syntax_check(char *line);
t_token					*tokenlist_add(t_token *list_start, int type, char *str,
							int quote_status);
int						not_a_word(char c1, char c2);
int						b_validate_token_sequence(t_token *token, t_data *data);
t_token					*tokenizer(char *line);

// expansion
int						expansion(t_token *list, t_data *data);
int						quote_handler(int quote_status, char c);
int						word_split(t_token *list);
int						append_env_var(char **out, char *word, int *i,
							t_data *data);
char					*append_str(char *dst, char *src);
char					*append_char(char *dst, char c);
int						get_var_name_len(char *word, int i);
int						consume_syntactic_quote(char c, int *quote_status);
int						expand_home_dir(t_token *list_elem, char **envp);
int						expand_dollar_question(char **out, int *i,
							t_data *data);

// word split
void					normalize_ifs_chars(char *s, char *ifs);
char					*ft_strchr_array(char *s, char *arr);
void					free_str_array(char **arr);
void					remove_implicit_null_arg(t_token **prev, t_token *next,
							t_token **list);

// execution
int						eggsecute(t_data *data);

// builtins
void					b_exit(t_data *data, t_cmd *cmd, int *saved_fds);

// singlecmds
void					f_redir_restore(int saved_fds[2], t_data *data);

// debug
void					print_tokens(t_token *start);
void					*test_calloc(size_t nmemb, size_t size);

# include "f_minishell/builtin/cd/f_cd.h"
# include "f_minishell/builtin/echo/f_echo.h"
# include "f_minishell/builtin/env/f_env.h"
# include "f_minishell/builtin/export/f_export.h"
# include "f_minishell/builtin/pwd/f_pwd.h"
# include "f_minishell/builtin/unset/f_unset.h"
# include "f_minishell/envp/f_envp.h"
# include "f_minishell/execution/f_exe.h"

#endif
