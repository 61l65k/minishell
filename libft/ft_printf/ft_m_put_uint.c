/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_l_put_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 23:47:37 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/03 18:05:13 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_m_put_uint(unsigned int dec)
{
	char	numbuff[100];
	int		temp;
	int		i;
	int		flag;

	flag = 0;
	i = 0;
	if (dec == 0)
		numbuff[i++] = '0';
	while (dec != 0)
	{
		temp = dec % 10;
		temp = temp + '0';
		numbuff[i++] = temp;
		dec /= 10;
	}
	numbuff[i] = '\0';
	return (ft_m_writeback(numbuff, i - 1, flag));
}
