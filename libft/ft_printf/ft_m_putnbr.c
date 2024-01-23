/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_l_putnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:46:46 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/17 16:03:10 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	check_minus(int *flag, int *dec)
{
	if (*dec == -2147483648)
	{
		if (write(1, "-2147483648", 11) < 0)
			return (-1);
		return (11);
	}
	if (*dec < 0)
	{
		if (ft_m_putchar('-') < 0)
			return (-1);
		*flag = 1;
		*dec *= -1;
	}
	return (0);
}

int	ft_m_putnbr(int dec)
{
	char	numbuff[100];
	int		temp;
	int		i;
	int		flag;
	int		err_ret;

	flag = 0;
	err_ret = check_minus(&flag, &dec);
	i = 0;
	if (err_ret != 0)
		return (err_ret);
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
