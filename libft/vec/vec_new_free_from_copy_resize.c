/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_new_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:15:55 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/12 15:09:44 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

int	vec_new(t_vec *dst, size_t init_len, size_t elem_size)
{
	if (!dst || elem_size == 0)
		return (-1);
	dst->alloc_size = init_len * elem_size;
	dst->len = 0;
	dst->elem_size = elem_size;
	if (init_len == 0)
		dst->memory = NULL;
	else
	{
		dst->memory = malloc(dst->alloc_size);
		if (!dst->memory)
		{
			dst->alloc_size = 0;
			return (-1);
		}
	}
	return (1);
}

void	vec_free(t_vec *src)
{
	if (src->memory)
		free(src->memory);
	src->memory = NULL;
	src->len = 0;
	src->alloc_size = 0;
	src->elem_size = 0;
}

void	*ft_memcpy(void *dst, const void *src, size_t n);

int	vec_from(t_vec *dst, void *src, size_t len, size_t elem_size)
{
	if (!dst || !src || elem_size == 0)
		return (-1);
	if (vec_new(dst, len, elem_size) == -1)
		return (-1);
	ft_memcpy(dst->memory, src, dst->alloc_size);
	dst->len = len;
	return (1);
}

int	vec_copy(t_vec *dst, t_vec *src)
{
	size_t	copy_size;

	if (!dst || !src || !src->memory)
		return (-1);
	else if (!dst->memory)
		vec_new(dst, src->len, src->elem_size);
	if (dst->alloc_size == 0)
		return (-1);
	if (src->len * src->elem_size < dst->alloc_size)
		copy_size = src->len * src->elem_size;
	else
		copy_size = dst->alloc_size;
	ft_memcpy(dst->memory, src->memory, copy_size);
	dst->len = copy_size / dst->elem_size;
	return (1);
}

int	vec_resize(t_vec *src, size_t target_len)
{
	t_vec	dst;

	if (!src)
		return (-1);
	else if (!src->memory)
		return (vec_new(src, target_len, src->elem_size));
	else if (vec_new(&dst, target_len, src->elem_size) == -1)
		return (-1);
	ft_memcpy(dst.memory, src->memory, src->len * src->elem_size);
	dst.len = src->len;
	vec_free(src);
	*src = dst;
	return (1);
}
