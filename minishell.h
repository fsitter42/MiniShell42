/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:38:55 by slambert          #+#    #+#             */
/*   Updated: 2026/04/14 15:12:49 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define _POSIX_C_SOURCE 200809L
# define MAX_ENV_LEN 600

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
# include <signal.h>
#include <sys/ioctl.h>

extern volatile sig_atomic_t g_signal_received;

typedef struct s_envl	t_envl;
typedef struct s_envp	t_envp;
typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;
typedef struct s_cd t_cd;

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
	int					ret_from_hsl;
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

typedef struct s_cd
{
	char	*old_pwd;
	char	*new_pwd;
	char	*export_str;
	char	*target;
    int     has_target;
}			t_cd;

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
int						word_split(t_token *list, t_data *data);
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
int	create_and_fill_new_tokens(char **split_result, t_token *list);
int	normalizer(char *str, char *ifs, char ***split_result);
int	cleanup_ifs_and_return(char *ifs, int ifs_is_allocated, int ret);
int	is_implicit_null_word(t_token *list);
int	replace_first_split_word(t_token *list, char **split_result);

// execution
int						eggsecute(t_data *data);

// builtins
void					b_exit(t_data *data, t_cmd *cmd, int *saved_fds);

// singlecmds
void					f_redir_restore(int saved_fds[2], t_data *data);

// debug
void					print_tokens(t_token *start);
void					*test_calloc(size_t nmemb, size_t size);

// f_cd.c
int			f_cd(t_data *data, char **args);

// f_echo.c
int	f_echo(t_data *data, char **args);

// f_env.c
int	f_env(t_data *data, char **args);
int	f_print_env(t_envl *list_head);

// f_export_no_key.c
int		f_print_export(t_envl *original_list, t_data *data);
t_envl	*f_copy_list(t_envl *head);
void	f_sort_env_list(t_envl *head);
int		f_strcmp(const char *s1, const char *s2);

// f_export_key.c
int		f_lst_add_one(t_envl *list, char *key, char *val);
void	f_free_node(t_envl *node);
int		f_valid_identifier(char *s);
int		f_export_with_key(t_envl *envl, char *s, t_data *data, int *err);

// f_export.c
int		f_export(t_data *data, char **args);

// f_pwd.c
int	f_pwd(t_data *data);

// f_unset.c
int		f_unset(t_data *data, char **args);
int		f_lst_del_one(t_envl **list, char *key);
char	*f_get_env_val(t_envl *list, char *key, t_data *data);

// f_envp_to_lst.c
t_envl	*f_copy_env(char **envp);
t_envl	*f_new_node(void);
t_envl	*f_free_env_list(t_envl *head);
int		f_fill_node(t_envl *node, char *env_str);

// f_lst_to_new_envp.c
int		f_update_envp(t_envp *data, t_data *bigdata);
char	**f_free_envp(char **envp);
size_t	f_lst_len_active(t_envl *list);
char	**f_envp_lst_to_p(t_envl *list, size_t keylen, size_t vallen, size_t i);

// f_envp_init_destroy.c
t_envp	*f_init_envp(char **envp);
t_envp	*f_destroy_envp(t_envp *p);

// f_pathfinder.c
char	*f_find_path(char *cmd, char **envp, int *err, int *err2);
void	f_validate_path(char *path, int *err, int *err2);

// f_pathhandler.c
char	*f_path_handler(t_data *data, char *cmd, char **envp);
void	f_print_error(char *context, char *msg);

// f_unsetpath
int f_path_set(t_data *data);

// f_singlecmd.c
void	f_exec_cmd(t_data *data, t_cmd *cmd, char **envp);
int		f_is_builtin(char *cmd);
int		f_exec_builtin(t_cmd *cmd, t_data *data);

// f_exe.c
int		f_exec_pipeline(t_data *data, t_cmd *cmd, int pipe_fd[2]);
// int		f_exec_pipeline(t_data *data, t_cmd *cmds);

// f_exe2.c
int		f_redir_wrapper(t_data *data, t_cmd *cmd);
int redir_return (t_data *data);

// b_heredoc.c
int		b_handle_heredoc(t_data *data, t_cmd *cmd, t_redir *redir);

// f_exec_wrapper.c
int		f_pipeline_wrapper(t_data *data);
void	f_close_pipe(t_cmd *cmd, int pipe_fd[2], int *prev_fd);
void	f_close_child(int *pipe_fd, int prev_fd, t_cmd *cmd);
void f_setup_pipe_fds(t_data *data, t_cmd *cmd, int *prev_fd, int *pipe_fd);
void f_setup_cmd_fds(t_data *data, t_cmd *cmd, int *pipe_fd, int prev_fd);

// f_exec_error.c
int	f_pipe_error(t_data *data, t_cmd *cmd, int *prev_fd);
int	f_fork_error(t_data *data, t_cmd *cmd, int pipe_fd[2], int *prev_fd);
void f_dup_error(t_data *data, t_cmd *cmd, int *pipe_fd, int prev_fd);
int f_redir_error(int saved_fds[2], int fd, char *err);

// f_signal.c
void	f_setup_signals(void);
void set_exit_code_to_130_and_free(t_data *data, char *line);

void beidl();

#endif
