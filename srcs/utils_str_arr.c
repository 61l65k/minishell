/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str_arr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:27:06 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/29 14:39:24 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_str_array(char **str_arr)
{
	char	**tmp;

	tmp = str_arr;
	while (tmp && *tmp)
	{
		free(*tmp);
		*tmp = NULL;
		tmp++;
	}
	free(str_arr);
}

/**
 * Returns malloced duplicate of a string array,
 * or NULL if malloc fails.
 */
char	**ft_strdup_array(const char **arr)
{
	int		i;
	char	**new;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		new[i] = ft_strdup(arr[i]);
		if (!new[i])
		{
			free_str_array(new);
			return (NULL);
		}
		i++;
	}
	new[i] = NULL;
	return (new);
}

size_t	str_arr_len(const char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

/**
 * Returns a new string array with the given string appended to the end,
 * or NULL if malloc fails.
 * Returned array and all its strings are malloced.
 */
char	**strarrjoin(const char **arr, const char *str)
{
	int		i;
	char	**new;

	if (!arr)
		return (NULL);
	new = malloc(sizeof(char *) * (str_arr_len(arr) + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		new[i] = ft_strdup(arr[i]);
		if (!new[i])
			return (free_str_array(new), NULL);
		i++;
	}
	new[i] = ft_strdup(str);
	if (!new[i])
		return (free_str_array(new), NULL);
	new[i + 1] = NULL;
	return (new);
}
