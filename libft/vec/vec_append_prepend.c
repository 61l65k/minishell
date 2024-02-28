/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_append_prepend.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:15:55 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/12 18:05:07 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

void	*ft_memcpy(void *dst, const void *src, size_t n);

int	vec_append(t_vec *dst, t_vec *src)
{
	size_t	new_len;
	int		ret;

	if (!dst || !src || !src->memory)
		return (-1);
	if (!dst->memory)
	{
		if (vec_new(dst, 1, src->elem_size) == -1)
			return (-1);
	}
	new_len = dst->len + src->len;
	if (dst->alloc_size < new_len * dst->elem_size)
	{
		if (new_len * dst->elem_size < SIZE_MAX / 2)
			ret = vec_resize(dst, new_len * 2);
		else
			ret = vec_resize(dst, new_len);
		if (ret == -1)
			return (-1);
	}
	ft_memcpy(
		&dst->memory[dst->elem_size * dst->len],
		src->memory, src->len * src->elem_size);
	dst->len += src->len;
	return (1);
}

int	vec_prepend(t_vec *dst, t_vec *src)
{
	t_vec	result;

	if (!dst || !src || !src->memory)
		return (-1);
	if (!dst->memory)
	{
		if (vec_new(dst, 1, src->elem_size) == -1)
			return (-1);
	}
	if (vec_new(&result, dst->len + src->len, dst->elem_size) == -1)
		return (-1);
	vec_copy(&result, src);
	ft_memcpy(
		&result.memory[src->len * src->elem_size],
		dst->memory,
		dst->len * dst->elem_size);
	vec_free(dst);
	*dst = result;
	return (1);
}
