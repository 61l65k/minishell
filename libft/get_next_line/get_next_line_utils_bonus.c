/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 16:03:20 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/17 14:13:01 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	slen_b(char *s)
{
	size_t		len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static void	join_buffs_b(char *join, char *tmp, char *bf)
{
	ssize_t			ti;
	ssize_t			bf_i;

	if (join)
	{
		ti = -1;
		bf_i = 0;
		while (tmp[++ti])
			join[ti] = tmp[ti];
		while (bf[bf_i])
			join[ti++] = bf[bf_i++];
		join[ti] = '\0';
	}
}

char	*join_free_b(char *tmp, char *bf)
{
	char			*join;
	size_t			total_len;

	if (!tmp)
	{
		tmp = (char *)malloc(1);
		if (!tmp)
			return (NULL);
		tmp[0] = '\0';
	}
	if (!bf)
		return (tmp);
	total_len = slen_b(tmp) + slen_b(bf) + 1;
	join = (char *)malloc((total_len));
	if (!join)
	{
		if (tmp)
			free(tmp);
		return (NULL);
	}
	join_buffs_b(join, tmp, bf);
	free(tmp);
	tmp = 0;
	return (join);
}

ssize_t	find_nl_b(char *str)
{
	ssize_t		idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '\n')
			return (idx);
		idx++;
	}
	return (-1);
}
