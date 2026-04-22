/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:15:57 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/22 20:39:33 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*b_create_heredoc_path(int id)
{
	char	*filename;
	char	*id_as_string;

	id_as_string = ft_itoa(id);
	if (!id_as_string)
		return (NULL);
	filename = ft_strjoin("/tmp/.heredoc_dump_", id_as_string);
	if (!filename)
		return (free(id_as_string), NULL);
	free(id_as_string);
	return (filename);
}

static int	expand_heredoc_line(t_data *data, char **line)
{
	char	*expanded;
	int		ret;

	if (!line || !*line)
		return (RET_OK);
	expanded = expand_word_one_pass(*line, data, TRUE, &ret);
	if (!expanded)
	{
		free(*line);
		*line = NULL;
		return (ret);
	}
	free(*line);
	*line = expanded;
	return (RET_OK);
}

static int	is_delimiter_line(char *line, t_redir *redir)
{
	if (ft_strlen(line) == ft_strlen(redir->delimiter) && ft_strncmp(line,
			redir->delimiter, ft_strlen(redir->delimiter)) == 0)
	{
		free(line);
		return (TRUE);
	}
	return (FALSE);
}

static int	b_heredoc_loop(t_data *data, t_redir *redir, char *filename, int fd)
{
	char	*line;

	while (1)
	{
		line = readline(">");
		if (g_signal_received == SIGINT)
		{
			set_exit_code_to_130_and_free(data, line);
			return (unlink(filename), -1);
		}
		if (!line)
			break ;
		if (is_delimiter_line(line, redir))
			break ;
		if (!redir->delimiter_is_quoted)
			if (expand_heredoc_line(data, &line) != RET_OK)
				return (unlink(filename), -1);
		if (write(fd, line, ft_strlen(line)) == -1)
			return (-1);
		if (write(fd, "\n", 1) == -1)
			return (-1);
		free(line);
	}
	return (0);
}

int	b_handle_heredoc(t_data *data, t_redir *redir)
{
	int		fd;
	char	*filename;

	filename = b_create_heredoc_path(redir->id);
	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (unlink(filename), free(filename), -1);
	if (b_heredoc_loop(data, redir, filename, fd) == -1)
		return (close(fd), free(filename), -1);
	if (close(fd) == -1)
		return (unlink(filename), free(filename), -1);
	redir->file = filename;
	return (0);
}
