/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 08:12:59 by apyykone          #+#    #+#             */
/*   Updated: 2024/01/11 08:14:25 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	long long	sign;
	long long	res;

	sign = 1;
	res = 0;
	while (*str == ' ' || ((*str >= 9) && (*str <= 13)))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = (res * 10) + (*str - '0');
		if (res < 0 && sign == 1)
			return (LONG_MIN);
		else if (res < 0 && sign == -1)
			return (LONG_MAX);
		str++;
	}
	return ((long)res * sign);
}
