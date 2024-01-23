/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:13:29 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/26 23:56:52 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static int	handle(const char *s1, size_t *start, char const *set, size_t *end)
{
	while (is_in_set(s1[*start], set))
		*start += 1;
	if (*start >= *end)
		return (-1);
	while (is_in_set(s1[*end - 1], set))
		*end -= 1;
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*result;
	size_t	i;

	if (!s1 || !set)
		return (ft_strdup(""));
	start = 0;
	end = ft_strlen(s1);
	i = 0;
	if (handle(s1, &start, set, &end) == -1)
		return (ft_strdup(""));
	len = end - start;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	while (i < len)
	{
		result[i] = s1[start + i];
		i++;
	}
	result[len] = '\0';
	return (result);
}
