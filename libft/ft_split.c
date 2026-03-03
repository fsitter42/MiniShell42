/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 08:47:56 by fsitter           #+#    #+#             */
/*   Updated: 2025/11/12 13:54:58 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			**ft_split(char const *s, char c);
static size_t	how_many_words(char const *si, char ci);
static char		**make_words(char **pointerzuwords, const char *s, char c);
static void		*free_delete(char **pointerzuwords, size_t word);

static void	*free_delete(char **pointerzuwords, size_t word)
{
	size_t	i;

	i = 0;
	while (i < word)
	{
		free(pointerzuwords[i]);
		i++;
	}
	free(pointerzuwords);
	return (NULL);
}

static char	**make_words(char **pointerzuwords, const char *s, char c)
{
	unsigned int	i;
	size_t			len;
	size_t			word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] && s[i] == c)
			i++;
		else
		{
			len = 0;
			while (s[i + len] && s[i + len] != c)
				len++;
			pointerzuwords[word] = ft_substr(s, i, len);
			if (!pointerzuwords[word])
				return (free_delete(pointerzuwords, word));
			i += len;
			word++;
		}
	}
	pointerzuwords[word] = NULL;
	return (pointerzuwords);
}

static size_t	how_many_words(char const *si, char ci)
{
	size_t				i;
	size_t				words;
	const unsigned char	*s;
	unsigned char		c;

	s = (const unsigned char *)si;
	c = (unsigned char)ci;
	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			words++;
			while (s[i] && s[i] != c)
				i++;
		}
		if (s[i] == c)
			i++;
	}
	return (words);
}

char	**ft_split(char const *s, char c)
{
	char	**pointerzuwords;

	if (!s)
		return (NULL);
	pointerzuwords = ft_calloc(sizeof(char *), (how_many_words(s, c) + 1));
	if (!pointerzuwords)
		return (NULL);
	pointerzuwords = make_words(pointerzuwords, s, c);
	if (!pointerzuwords)
		return (NULL);
	return (pointerzuwords);
}
