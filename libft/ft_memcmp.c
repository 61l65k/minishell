/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:41:52 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/28 11:00:21 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1p;
	unsigned char	*s2p;

	s1p = (unsigned char *) s1;
	s2p = (unsigned char *) s2;
	while (n > 0)
	{
		if (*s1p != *s2p)
			return (*s1p - *s2p);
		n--;
		s1p++;
		s2p++;
	}
	return (0);
}
