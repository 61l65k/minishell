/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 00:11:36 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/03 17:53:08 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	bytes_total;

	if (!nmemb || !size)
		return (ft_calloc(1, 1));
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	bytes_total = nmemb * size;
	if (bytes_total >= 4611686014132420609)
		return (NULL);
	ptr = malloc(bytes_total);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, bytes_total);
	return (ptr);
}
