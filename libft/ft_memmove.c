/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 17:25:12 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/30 11:57:39 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*pdest;
	const char	*psrc;
	size_t		i;

	if (dst == NULL && src == NULL)
		return (NULL);
	i = 0;
	pdest = (char *)dst;
	psrc = (const char *)src;
	if ((psrc < pdest) && (psrc + len > pdest))
	{
		pdest += len;
		psrc += len;
		while (len > 0)
		{
			*(--pdest) = *(--psrc);
			len--;
		}
	}
	while (i < len)
	{
		*(pdest + i) = *(psrc + i);
		i++;
	}
	return (dst);
}
