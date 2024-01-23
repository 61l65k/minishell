/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:20:19 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/28 11:18:13 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ret;
	char	ch;

	ret = (char *)s;
	ch = (char)c;
	while (*ret)
	{
		if (*ret == ch)
			return (ret);
		ret++;
	}
	if (ch == '\0')
		return (ret);
	return (NULL);
}
