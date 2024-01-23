/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 21:12:29 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/26 17:13:02 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*answ;
	size_t	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	answ = ft_strdup(s);
	if (!answ)
		return (NULL);
	while (answ[i])
	{
		(answ[i] = (*f)(i, answ[i]));
		i++;
	}
	return (answ);
}
