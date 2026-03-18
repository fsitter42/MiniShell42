/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exe2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:01:22 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/18 11:37:52 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	f_open_infile(t_data *data, t_cmd *cmd, char *file);
static int	f_open_outfile(t_data *data, t_cmd *cmd, char *file, int append);

int	f_open_redirections(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	cmd->in_fd = -1;
	cmd->out_fd = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (f_open_infile(data, cmd, redir->file) == -1)
				return (-1);
		}
		else if (redir->type == HEREDOC)
		{
			if (b_handle_heredoc(data, cmd, redir) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (f_open_outfile(data, cmd, redir->file,
					redir->type == REDIR_APPEND) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

static int	f_open_infile(t_data *data, t_cmd *cmd, char *file)
{
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	cmd->in_fd = open(file, O_RDONLY);
	if (cmd->in_fd == -1)
	{
		if (errno == ENOENT)
			f_print_error(file, "No such file or directory");
		else if (errno == EACCES)
			f_print_error(file, "Permission denied");
		else
			f_print_error(file, strerror(errno));
		data->last_exit_code = 1;
		return (-1);
	}
	return (0);
}

static char	*b_create_heredoc_filename(int id)
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
static int	b_handle_heredoc(t_data *data, t_cmd *cmd, t_redir *redir)
{
	int		fd;
	char	*line;
	char	*filename;

	(void)data;
	filename = b_create_heredoc_filename(redir->id);
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
	free(filename);
	if (cmd->in_fd == -1)
		return (-1);
	return (0);
}

static int	f_open_outfile(t_data *data, t_cmd *cmd, char *file, int append)
{
	if (cmd->out_fd != -1)
		close(cmd->out_fd);
	if (append)
		cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
	{
		if (errno == EACCES)
			f_print_error(file, "Permission denied");
		else
			f_print_error(file, strerror(errno));
		data->last_exit_code = 1;
		return (-1);
	}
	return (0);
}

/*n*/