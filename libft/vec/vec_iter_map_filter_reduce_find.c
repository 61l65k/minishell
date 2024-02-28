/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_iter_map_filter_reduce_find.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:31:48 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/12 20:01:47 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

void	vec_iter(t_vec *src, void (*f) (void *))
{
	size_t	i;

	if (!src || !src->memory || !f)
		return ;
	i = 0;
	while (i < src->len)
	{
		f(vec_get(src, i));
		i++;
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n);

int	vec_map(t_vec *dst, t_vec *src, void (*f) (void *))
{
	void	*ptr;
	void	*res;
	size_t	i;

	if (!dst || !src || !src->memory || !f)
		return (-1);
	else if (!dst->memory)
		vec_new(dst, 1, dst->elem_size);
	res = malloc(dst->elem_size);
	if (!res)
		return (-1);
	i = 0;
	while (i < src->len)
	{
		ptr = vec_get(src, i);
		ft_memcpy(res, ptr, dst->elem_size);
		f(res);
		vec_push(dst, res);
		i++;
	}
	free(res);
	return (1);
}

int	vec_filter(t_vec *dst, t_vec *src, bool (*f) (void *))
{
	void	*ptr;
	void	*res;
	size_t	i;

	if (!dst || !src || !src->memory || !f)
		return (-1);
	else if (!dst->memory)
		vec_new(dst, 1, dst->elem_size);
	res = malloc(dst->elem_size);
	if (!res)
		return (-1);
	i = 0;
	while (i < src->len)
	{
		ptr = vec_get(src, i);
		ft_memcpy(res, ptr, dst->elem_size);
		if (f(res) == true)
			vec_push(dst, res);
		i++;
	}
	free(res);
	return (1);
}

int	vec_reduce(void *dst, t_vec *src, void (*f) (void *, void *))
{
	void	*ptr;
	size_t	i;

	if (!dst || !src || !src->memory || !f)
		return (-1);
	i = 0;
	while (i < src->len)
	{
		ptr = vec_get(src, i);
		f(dst, ptr);
		i++;
	}
	return (1);
}

void	*vec_find(t_vec *src, bool (*f) (void *))
{
	void	*ptr;
	size_t	i;

	if (!src || !src->memory)
		return (NULL);
	i = 0;
	while (i < src->len)
	{
		ptr = vec_get(src, i);
		if (f(ptr) == true)
			return (ptr);
		i++;
	}
	return (NULL);
}
