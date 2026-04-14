/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:52:01 by slambert          #+#    #+#             */
/*   Updated: 2026/04/14 15:16:50 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

//int					f_is_syntax_valid(t_data *data);
void				*test_calloc(size_t nmemb, size_t size);

void	*test_calloc(size_t nmemb, size_t size)
{
	(void)nmemb;
	(void)size;
	return (NULL);
}

static int	hsl_helper(t_token *token_list, t_cmd *cmd_list, t_data *data)
{
	int	ret;

	ret = RET_OK;
	if (!is_token_list_empty(token_list))
	{
		ret = create_command_list(token_list->next, &cmd_list);
		if (ret != RET_OK)
			return (cleanup_token_list(token_list), ret);
		cleanup_token_list(token_list);
		data->cmds = cmd_list;
		ret = eggsecute(data);
	}
	else
	{
		data->e_has_been_set = 1;
		cleanup_token_list(token_list);
	}
	if (ret == 1 && data->should_exit == 1)
		return (ERROR_HARD);
	cleanup_command_list(cmd_list);
	data->cmds = NULL;
	data->e_has_been_set = 1;
	return (RET_OK);
}
// das war vor eggsecute call
/* 		ret = f_is_syntax_valid(data);
		if (ret != RET_OK)
		{
			cleanup_command_list(cmd_list);
			data->cmds = NULL;
			data->e_has_been_set = 1;
			return (ret);
		} */

static int	handle_single_line(char *line, t_data *data)
{
	int		ret;
	t_token	*token_list;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	token_list = tokenizer(line);
	if (!token_list)
		return (ERROR_HARD);
	ret = b_validate_token_sequence(token_list->next, data);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	ret = find_delimiters(token_list);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	ret = expansion(token_list, data);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	ret = word_split(token_list, data);
	if (ret != RET_OK)
		return (cleanup_token_list(token_list), ret);
	return (hsl_helper(token_list, cmd_list, data));
}

static void print_and_free_line(char *line)
{
	printf("minishell: missing quote\n");
	free (line);
}

static void errno_and_exit_code_helper(t_data *data)
{
	if (errno != 0)
		data->last_exit_code = 1;
	ft_putendl_fd("exit", 1);
}

static int add_to_history_and_hsl(char *line, t_data *data)
{
	int ret;
	
	if (*line && !line_is_empty(line))
		add_history((const char *)line);
	ret = handle_single_line(line, data);
	free(line);
	return (ret);
}


/* this is the default mode in where the users enters stuff
 */
void	normal_mode(t_data *data)
{
	char	*line;
	
	while (1)
	{
		errno = 0;
		line = readline("minishell$ ");
		if (g_signal_received == SIGINT)
		{
			set_exit_code_to_130_and_free(data, line);
			continue;
		}
		if (!line)
		{
			errno_and_exit_code_helper(data);
			break ;
		}
		if (quote_syntax_check(line))
		{
			print_and_free_line(line);
			continue ;
		}
		data->ret_from_hsl = add_to_history_and_hsl(line, data);
		if (data->ret_from_hsl == ERROR_HARD)
			break ;
	}
}

void	debug_mode(char *input, t_data *data)
{
	int		i;
	int		ret;

	data->strs = NULL;
	data->strs = ft_split(input, ';');
	if (!data->strs)
		return ;
	i = -1;
	while (data->strs[++i])
	{
		ret = handle_single_line(data->strs[i], data);
		if (ret == ERROR_HARD)
		{
			cleanup_split_result(data->strs, 0);
			data->strs = NULL;
			return ;
		}
		if (ret == ERROR_SOFT)
			continue ;
		data->cmds = NULL;
	}
	cleanup_split_result(data->strs, 0);
	data->strs = NULL;
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
		f_setup_signals();
		normal_mode(data);
	}
	else
	{
		if (ft_strncmp(argv[1], "-d", 2) != 0)
			return (printf("wrong syntax - did you use the -d flag?\n"), 1);
		data = sfbf_init_all(envp);
		if (!data)
			return (1);
		debug_mode(argv[2], data);
	}
	last_exit_code = data->last_exit_code;
	sfbf_free_all(data);
	return (last_exit_code);
}
