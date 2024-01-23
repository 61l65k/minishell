/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 18:17:46 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/24 17:49:52 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s;
	size_t	slen;

	slen = ft_strlen(s1);
	s = (char *)malloc(sizeof(char) * (slen + 1));
	if (!s)
		return (NULL);
	ft_memcpy(s, s1, slen);
	s[slen] = '\0';
	return (s);
}
