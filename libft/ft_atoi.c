/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:56:23 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/30 12:29:29 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long long	sign;
	long long	res;

	sign = 1;
	res = 0;
	while (*str == ' ' || ((9 <= *str) && (*str <= 13)))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = (res * 10) + (*str++ - '0');
		if (res < 0 && sign == 1)
			return (-1);
		else if (res < 0 && sign == -1)
			return (0);
	}
	return ((int)res * sign);
}
