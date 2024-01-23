/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 23:44:57 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/01 11:53:58 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	answlen;
	size_t	i;

	if (dstsize == 0)
		return (ft_strlen(src));
	if (dstsize <= ft_strlen(dst))
		return (dstsize + ft_strlen(src));
	answlen = ft_strlen(dst);
	i = 0;
	while (src[i] != '\0' && answlen + 1 < dstsize)
	{
		dst[answlen] = src[i];
		i++;
		answlen++;
	}
	dst[answlen] = '\0';
	return (ft_strlen(dst) + ft_strlen(&src[i]));
}
