/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:42:13 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 16:42:14 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s;
	size_t	slen;
	size_t	copy_len;

	slen = ft_strlen(s1);
	if (n > slen)
		copy_len = slen;
	else
		copy_len = n;
	s = (char *)malloc(sizeof(char) * (copy_len + 1));
	if (!s)
		return (NULL);
	ft_memcpy(s, s1, copy_len);
	s[copy_len] = '\0';
	return (s);
}
