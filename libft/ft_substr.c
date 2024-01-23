/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:53:45 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/28 14:10:47 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	size_t	i;
	char	*answ;

	if (s == NULL )
		return (NULL);
	i = 0;
	slen = ft_strlen(s);
	if (start >= slen || len == 0)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	answ = (char *)malloc(sizeof(char) * len + 1);
	if (!answ)
		return (NULL);
	answ[len] = '\0';
	if (start < slen)
	{
		while (i < len)
			answ[i++] = s[start++];
	}
	return (answ);
}
