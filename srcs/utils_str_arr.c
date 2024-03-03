/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str_arr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:27:06 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/03 01:56:52 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Frees and nulls each string in the given string array,
 *	and then frees the array itself. In case NULL is passed, nothing happens.
 * @param str_arr The null terminated string array to be freed.
 */
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
 * @brief Frees and nulls each string and finally frees and nulls
 *	the string array itself. In case NULL is passed, nothing happens.
 * @param str_arr_ptr The pointer to the string array to be freed.
 */
void	free_and_null_str_array(char ***str_arr_ptr)
{
	if (str_arr_ptr && *str_arr_ptr)
	{
		free_str_array(*str_arr_ptr);
		*str_arr_ptr = NULL;
	}
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
