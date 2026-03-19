/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:15:57 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/19 14:18:02 by fsitter          ###   ########.fr       */
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
	filename = ft_strjoin("./.heredoc_dump_", id_as_string);
	if (!filename)
		return (free(id_as_string), NULL);
	free(id_as_string);
	return (filename);
}

static int	b_heredoc_loop(t_cmd *cmd, t_redir *redir, char *filename, int fd)
{
	char	*line;

	while (1)
	{
		line = readline(">");
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(redir->delimiter) && ft_strncmp(line,
				redir->delimiter, ft_strlen(redir->delimiter)) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			return (-1);
		if (write(fd, "\n", 1) == -1)
			return (-1);
		free(line);
	}
	return (0);
}

/*

 *   TODO when input is << d no heredocing is happening
 *   TODO the above case prints the heredoced stuff back to the user,
 *   i have to do that too (find out in what cases exactly)
 *	 TODO hier was mit last exit code?
 *	 TODO den schas wieder löschen
 */
int	b_handle_heredoc(t_data *data, t_cmd *cmd, t_redir *redir)
{
	int		fd;
	char	*line;
	char	*filename;

	(void)data;
	filename = b_create_heredoc_path(redir->id);
	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (free(filename), -1);
	if (b_heredoc_loop(cmd, redir, filename, fd) == -1)
		return (free(line), close(fd), free(filename), -1);
	if (close(fd) == -1)
		return (free(filename), -1);
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	cmd->in_fd = open(filename, O_RDONLY);
	// redir->file = ft_strdup(filename);
	// free(filename);
	// if (!redir->file)
	// 	return -1;
	if (cmd->in_fd == -1)
		return (-1);
	redir->file = filename;
	return (0);
}
