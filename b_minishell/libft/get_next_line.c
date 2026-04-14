/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:09:53 by dabdulla          #+#    #+#             */
/*   Updated: 2026/02/19 12:49:06 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*free_and_null(char **s)
{
	if (*s)
	{
		free(*s);
		*s = NULL;
	}
	return (NULL);
}

int	ft_strlen_int(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup_gnl(char *src)
{
	int		i;
	char	*dupsrc;

	i = 0;
	dupsrc = malloc(ft_strlen_int(src) + 1);
	if (!dupsrc)
		return (NULL);
	while (src[i])
	{
		dupsrc[i] = src[i];
		i++;
	}
	dupsrc[i] = '\0';
	return (dupsrc);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new_arr;

	i = 0;
	new_arr = malloc((ft_strlen_int(s1) + ft_strlen_int(s2)) + 1);
	if (!new_arr)
		return (free_and_null(&s1), NULL);
	while (s1[i])
	{
		new_arr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new_arr[i] = s2[j];
		i++;
		j++;
	}
	if (s1)
		free(s1);
	new_arr[i] = '\0';
	return (new_arr);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	slen = ft_strlen_int(s);
	if (start >= slen)
		return (ft_strdup_gnl(""));
	if (len > slen - start)
		len = slen - start;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[len] = '\0';
	return (str);
}

char	*join_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin_gnl(s1, s2);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	find_new_line(char *str)
{
	int	i;

	i = 0;
	if (*str == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

int	is_newline(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

char	*read_loop(int fd, char *file)
{
	char	*buffer;
	int		r;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_and_null(&file));
	if (!file)
	{
		file = malloc(1);
		if (!file)
			return (free_and_null(&buffer));
		file[0] = '\0';
	}
	r = 1;
	while (r > 0 && !is_newline(file))
	{
		r = read(fd, buffer, BUFFER_SIZE);
		if (r <= 0)
			break ;
		buffer[r] = '\0';
		file = join_free(file, buffer);
		if (!file)
			return (free_and_null(&buffer), free_and_null(&file));
	}
	return (free_and_null(&buffer), file);
}

char	*get_next_line(int fd)
{
	static char	*file;
	char		*tmp;
	char		*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_and_null(&file));
	file = read_loop(fd, file);
	if (!file)
		return (NULL);
	if (*file == '\0')
		return (free_and_null(&file));
	tmp = ft_substr(file, 0, find_new_line(file));
	if (!tmp)
		return (free_and_null(&file), free_and_null(&tmp));
	str = ft_substr(file, find_new_line(tmp), ft_strlen_int(file)
			- ft_strlen_int(tmp));
	free_and_null(&file);
	if (!str)
		return (free_and_null(&tmp));
	file = ft_strdup_gnl(str);
	if (!file)
		return (free_and_null(&str), free_and_null(&file), free_and_null(&tmp));
	free_and_null(&str);
	return (tmp);
}
