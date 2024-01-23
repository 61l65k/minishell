/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_l_puthex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:59:29 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/09 12:22:54 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*** Writes buffer backwards algorithm adds hexdigits
 *		in backward order to the buffer				  */
int	ft_m_writeback(char *hexbuff, int start, int flag)
{
	int		len;

	len = 0;
	while (start >= 0)
	{
		if (ft_m_putchar(hexbuff[start--]) < 0)
			return (-1);
		len++;
	}
	if (flag)
		len++;
	return (len);
}

int	ft_m_puthex(unsigned int dec, char c)
{
	char	hexbuff[100];
	int		temp;
	int		i;

	i = 0;
	if (dec == 0)
		hexbuff[i++] = '0';
	while (dec != 0)
	{
		temp = dec % 16;
		if (temp < 10)
			temp = temp + '0';
		else
		{
			if (c == 'x')
				temp = temp + 87;
			else if (c == 'X')
				temp = temp + 55;
		}
		hexbuff[i++] = temp;
		dec /= 16;
	}
	hexbuff[i] = '\0';
	return (ft_m_writeback(hexbuff, i - 1, 0));
}
