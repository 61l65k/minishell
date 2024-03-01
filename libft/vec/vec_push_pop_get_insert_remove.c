/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_push_pop_get_insert_remove.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:15:55 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/01 21:23:09 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

void	*ft_memcpy(void *dst, const void *src, size_t n);

int	vec_push(t_vec *dst, void *src)
{
	if (!dst | !src)
		return (-1);
	if (!dst->memory)
	{
		if (vec_new(dst, 1, dst->elem_size) == -1)
		{
			return (-1);
		}
	}
	if (dst->elem_size * (dst->len + 1) > dst->alloc_size)
	{
		if (vec_resize(dst, dst->len * 2) == -1)
		{
			return (-1);
		}
	}
	ft_memcpy(&dst->memory[dst->elem_size * dst->len], src, dst->elem_size);
	dst->len++;
	return (1);
}

void	*vec_get(t_vec *src, size_t index)
{
	unsigned char	*ptr;

	if (!src || !src->memory || index >= src->len)
	{
		return (NULL);
	}
	ptr = &src->memory[src->elem_size * index];
	return (ptr);
}

int	vec_pop(void *dst, t_vec *src)
{
	if (!dst || !src)
		return (-1);
	if (!src->memory || src->len == 0)
		return (0);
	ft_memcpy(dst, vec_get(src, src->len - 1), src->elem_size);
	src->len--;
	return (1);
}

void	*ft_memmove(void *dst, const void *src, size_t n);

int	vec_insert(t_vec *dst, void *src, size_t index)
{
	if (!dst || !src || index > dst->len)
		return (-1);
	if (dst->len == index)
		return (vec_push(dst, src));
	if (dst->elem_size * (dst->len + 1) > dst->alloc_size)
	{
		if (vec_resize(dst, (dst->alloc_size * 2) / dst->elem_size) == -1)
		{
			return (-1);
		}
	}
	ft_memmove(
		vec_get(dst, index) + dst->elem_size,
		vec_get(dst, index),
		(dst->len - index) * dst->elem_size);
	ft_memcpy(
		vec_get(dst, index),
		src,
		dst->elem_size);
	dst->len++;
	return (1);
}

int	vec_remove(t_vec *src, size_t index)
{
	if (!src || index >= src->len)
		return (-1);
	if (index == src->len)
	{
		src->len--;
		return (1);
	}
	ft_memmove(
		vec_get(src, index),
		&src->memory[src->elem_size * (index + 1)],
		(src->len - index) * src->elem_size);
	src->len--;
	return (1);
}
