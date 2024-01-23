/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alex P <apyykone@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 22:37:26 by Alex P            #+#    #+#             */
/*   Updated: 2023/10/24 12:11:23 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*src;
	unsigned char		uc;
	size_t				i;

	i = 0;
	src = (const unsigned char *)s;
	uc = (unsigned char)c;
	while (i < n)
	{
		if (src[i] == uc)
			return ((void *)(src + i));
		i++;
	}
	return (NULL);
}
