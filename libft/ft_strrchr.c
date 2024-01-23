/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:32:32 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/28 11:21:16 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*sp;
	char	*last_occurrence;
	char	ch;

	ch = (char)c;
	sp = (char *)s;
	last_occurrence = NULL;
	while (*sp)
	{
		if (*sp == ch)
			last_occurrence = sp;
		sp++;
	}
	if (ch == '\0')
		return (sp);
	return (last_occurrence);
}
