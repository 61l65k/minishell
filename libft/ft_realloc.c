/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:33:01 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 20:33:02 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *old_ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	size_to_copy;

	if (new_size == 0)
	{
		free(old_ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_ptr)
	{
		if (old_size < new_size)
			size_to_copy = old_size;
		else
			size_to_copy = new_size;
		ft_memcpy(new_ptr, old_ptr, size_to_copy);
		free(old_ptr);
	}
	return (new_ptr);
}
