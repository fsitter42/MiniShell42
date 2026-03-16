/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 09:59:11 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 14:15:42 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd);
char	*read_line(int fd, char *text);
char	*join_and_free(char *text, char *buffer);
char	*get_line(char *text);
char	*clean_line(char *text);

char	*clean_line(char *text)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	j = 0;
	while (text[i] && text[i] != '\n')
		i++;
	if (!text[i])
	{
		free(text);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(text) - i + 1), sizeof(*text));
	if (!line)
		return (NULL);
	while (text[++i])
		line[j++] = text[i];
	line[j] = '\0';
	free(text);
	return (line);
}

char	*get_line(char *text)
{
	size_t	i;
	char	*line;

	i = 0;
	if (!text[i])
		return (NULL);
	while (text[i] && text[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	i = 0;
	while (text[i] && text[i] != '\n')
	{
		line[i] = text[i];
		i++;
	}
	if (text[i] && text[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	return (line);
}

char	*join_and_free(char *text, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(text, buffer);
	free(text);
	return (temp);
}

char	*read_line(int fd, char *text)
{
	char	*buffer;
	int		bytes_read;

	if (!text)
		text = ft_calloc(1, 1);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(text);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = 0;
		text = join_and_free(text, buffer);
		if (ft_strchr(text, '\n'))
			break ;
	}
	free(buffer);
	return (text);
}

char	*get_next_line(int fd)
{
	char		*output;
	static char	*text;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	text = read_line(fd, text);
	if (!text)
		return (NULL);
	output = get_line(text);
	text = clean_line(text);
	return (output);
}
