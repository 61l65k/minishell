/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:07:40 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/30 11:37:18 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;
	unsigned char	ch;

	ch = (unsigned char)c;
	p = (unsigned char *)b;
	while (len > 0)
	{
		*p++ = ch;
		len--;
	}
	return (b);
}
