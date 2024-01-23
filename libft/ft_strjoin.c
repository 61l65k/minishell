/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:01:55 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/27 00:01:27 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*answ;
	size_t	s1_len;
	size_t	s2_len;
	size_t	answ_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	answ_len = s1_len + s2_len;
	answ = (char *)malloc(sizeof(char) * (answ_len + 1));
	if (!answ)
		return (NULL);
	answ[answ_len] = '\0';
	ft_strlcpy(answ, s1, s1_len + 1);
	ft_strlcat(answ, s2, answ_len + 1);
	return (answ);
}
