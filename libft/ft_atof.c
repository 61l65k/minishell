/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:37:54 by apyykone          #+#    #+#             */
/*   Updated: 2024/01/11 12:37:55 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(const char *str)
{
	double	res;
	double	divider;
	double	neg;
	size_t	i;

	res = 0.0;
	divider = 0.1;
	neg = 1.0;
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '-')
	{
		neg = -1.0;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10.0 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.' || str[i] == ',')
	{
		i++;
		while (str[i] >= '0' && str[i] <= '9')
		{
			res += (str[i] - '0') * divider;
			divider /= 10.0;
			i++;
		}
	}
	return (res * neg);
}
