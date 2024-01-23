/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:16:52 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/01 12:38:44 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_splits(const char *s, char c)
{
	int	count;
	int	split;

	split = 1;
	count = 0;
	while (*s)
	{
		if (*s == c)
			split = 1;
		else if (split)
		{
			count++;
			split = 0;
		}
		s++;
	}
	return (count);
}

static char	**freeall(char **result, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(result[j++]);
	free(result);
	return (NULL);
}

static char	*duplicate_substring(const char *start, const char *end)
{
	int		length;
	char	*result;
	int		i;

	i = 0;
	length = end - start;
	result = (char *)malloc(length + 1);
	if (!result)
		return (NULL);
	while (i < length)
	{
		result[i] = start[i];
		i++;
	}
	result[length] = '\0';
	return (result);
}

static char	**split(char const *s, char c, char **result)
{
	int			i;
	const char	*start;

	i = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			start = s;
			while (*s && *s != c)
				s++;
			result[i] = duplicate_substring(start, s);
			if (!result[i])
				return (freeall(result, i));
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char		**result;
	int			num_splits;

	if (!s)
		return (NULL);
	num_splits = count_splits(s, c);
	result = (char **)malloc((num_splits + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	return (split(s, c, result));
}
