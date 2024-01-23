/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 23:29:03 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/26 16:19:31 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	size_t	srclen;
	size_t	destlen;
	size_t	i;

	srclen = 0;
	while (src[srclen] != '\0')
		srclen++;
	if (destsize == 0)
		return (srclen);
	destlen = destsize - 1;
	i = 0;
	while (i < destlen && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (srclen);
}
