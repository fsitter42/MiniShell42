/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:52:01 by slambert          #+#    #+#             */
/*   Updated: 2026/03/30 16:21:15 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*test_calloc(size_t nmemb, size_t size);

void	*test_calloc(size_t nmemb, size_t size)
{
	(void)nmemb;
	(void)size;
	return (NULL);
}

int			f_is_syntax_valid(t_data *data);
/*
	TODO:
	- handle exit status (should work now, test)
	- ctrl-C displays a new prompt on a new line
	- ctrl-D exits the shell
	- ctrl-\ does nothing
*/

int	is_token_list_empty(t_token *token_list)
{
	if (!token_list || !token_list->next)
	{
		// printf("Token list is empty\n");
		return (1);
	}
	return (0);
}

int	handle_delimiter(t_token *token_list)
{
	while (1)
	{
		while (token_list && token_list->type != HEREDOC)
			token_list = token_list->next;
		if (!token_list)
			return (RET_OK);
		if (token_list->next && token_list->next->type == WORD)
		{
			token_list->next->type = WORD_AFTER_HEREDOC;
			token_list = token_list->next;
		}
		else
			return (ERROR_SOFT);
	}
	return (RET_OK);
}

int	hsl_helper(t_token *token_list, t_cmd *cmd_list, t_data *data)
{
	int ret;

	ret = RET_OK;
	if (!is_token_list_empty(token_list))
	{
		cmd_list = create_command_list(token_list->next);
		if (!cmd_list)
			return (cleanup_token_list(token_list), ERROR_HARD);
		cleanup_token_list(token_list);
		data->cmds = cmd_list;
		ret = f_is_syntax_valid(data);
		if (ret != RET_OK)
		{
			cleanup_command_list(cmd_list);
			data->cmds = NULL;
			data->e_has_been_set = 1;
			return (ret);
		}
		ret = eggsecute(data); // frido geändert
	}
	else
	{
		data->e_has_been_set = 1;
		cleanup_token_list(token_list);
	}
	// TODO wir müssen überall data->should_exit auf 1 setzen in den builtins
	// alternativ ret -1 oder 600
	// oder wir ändern exec_ret auch auf ERROR_HARD (dann müssten wir return
	// von f_exec_builtin und f_pipeline_wrapper ändern und in allen dahinter
	// liegenden funktionen)
	if (ret == 1 && data->should_exit == 1)
		return (ERROR_HARD);
	cleanup_command_list(cmd_list);
	data->cmds = NULL;
	data->e_has_been_set = 1;
	return (RET_OK);
}

/* TODO think about return value. does 1 always mean something went wrong
 * (close minishell)  * or are there cases where nothing went wrong and we
 * just do nothing and give the input back to the user?
 * i think we need 3 states, OK, ERROR_RECOVERABLE (user gets input back)
 * and ERROR_FATAL (minishell exits and everything gets cleaned up)
 */
int	handle_single_line(char *line, char **envp, t_data *data)
{
	int		ret;
	t_token	*token_list;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	token_list = tokenizer(line); // token_list = test_calloc(1,1);  doublefree TODOS
	if (!token_list)
		return (ERROR_HARD);
	ret = handle_delimiter(token_list);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	ret = expansion(token_list, data);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	ret = word_split(token_list);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	return hsl_helper(token_list, cmd_list, data);
}

/* this is the default mode in where the users enters stuff
 */
void	normal_mode(int argc, char **argv, char **envp, t_data *data)
{
	char	*line;
	int		ret_from_hsl;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (ft_strncmp(line, "", 1) == 0)
		{
			free(line);
			continue ;
		}
		if (quote_sytanx_check(line))
		{
			printf("minishell: missing quote\n");
			continue;;
		}
		if (*line)
			add_history((const char *)line);
		ret_from_hsl = handle_single_line(line, envp, data);
		free(line);
		if (ret_from_hsl == ERROR_SOFT || ret_from_hsl == RET_OK)
			continue ;
		else if (ret_from_hsl == ERROR_HARD)
			break ;
	}
}

static void	cleanup_split_result(char **strs, int start)
{
	while (strs[start])
	{
		free(strs[start]);
		start++;
	}
	free(strs);
}

/* the difference here is that we have one string that is given to
 *  minishell, no interactions with the user. all lines have to be separated

	*  by a semicolon (;) and will be executed one afprintf("hello nur\n");ter each other.

	*  note: if we want to use a $ sign (var) we have to explicitly escape that character (\\ before §).

	*  otherwise the shell tries to expand the variable before it is given to minishell as an input
 */
void	debug_mode(char *input, char **envp, t_data *data)
{
	char	**strs;
	int		i;
	int		ret;

	strs = NULL;
	strs = ft_split(input, ';');
	if (!strs)
		return;
	data->strs = strs;
	i = -1;
	while (strs[++i])
	{
		ret = handle_single_line(strs[i], envp, data);
		if (ret == ERROR_HARD)
		{
			cleanup_split_result(strs, i);
			return ;
		}
		if (ret == ERROR_SOFT)
			continue ;
		data->cmds = NULL;
	}
}

// TODO $EMPTY echo hi does not print anything
void	sfbf_free_all(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->env->envp_lst)
		f_free_env_list(data->env->envp_lst); // vll nmoch freen zusätzliuch
	if (data->env->envp_updated)
		f_free_envp(data->env->envp_updated);
	// if (data->env->envp_ori)
	// 	free(data->env->envp_ori);
	free(data->env);
	i = 0;
	while (data->strs && data->strs[i])
	{
		free(data->strs[i]);
		i++;
	}
	if (data->strs)
		free(data->strs);
	cleanup_command_list(data->cmds);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	free(data);
}

t_data	*sfbf_init_all(char **envp)
{
	t_data	*data;
	char	**args;

	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
		return (NULL);
	data->env = f_init_envp(envp);
	if (!data->env)
		return (free(data), NULL);
	args = (char *[]){"export", "OLDPWD", NULL};
	if (f_export(data, args) == EXIT_FAILURE)
	{
		f_destroy_envp(data->env);
		free(data);
		return (NULL);
	}
	data->strs = NULL;
	data->last_exit_code = 0;
	data->should_exit = 0;
	data->e_has_been_set = 0;
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		last_exit_code;

	if (argc != 1 && argc != 3)
		return (printf("wrong syntax - argc not 1 or 3\n"), 1);
	if (argc == 1)
	{
		data = sfbf_init_all(envp);
		if (!data)
			return (1);
		normal_mode(argc, argv, envp, data);
	}
	else
	{
		if (ft_strncmp(argv[1], "-d", 2) != 0)
			return (printf("wrong syntax - did you use the -d flag?\n"), 1);
		data = sfbf_init_all(envp);
		if (!data)
			return (1);
		debug_mode(argv[2], envp, data);
	}
	sfbf_free_all(data);
}

int	f_is_syntax_valid(t_data *data)
{
	t_cmd	*tmp;

	tmp = data->cmds;
	while (tmp)
	{
		if (!tmp->cmd && !tmp->redirs)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token '|'\n",
				2);
			data->last_exit_code = 2;
			return (ERROR_SOFT);
		}
		tmp = tmp->next;
	}
	return (RET_OK);
}

/* size_t count_cmds(t_token *cmd_list)
{
	size_t size;

	size = 0;
	while (cmd_list)
	{
		size++;
		cmd_list = cmd_list->next;
	}
	return (size);
} */