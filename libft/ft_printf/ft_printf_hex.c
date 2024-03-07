/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 13:53:26 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 17:50:46 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

void	ft_printf_str(char *str, t_state *ret)
{
	if (str == NULL)
		ft_printf_str("(null)", ret);
	else
	{
		while (*str)
		{
			ft_printf_char(*str, ret);
			str++;
		}
	}
}

void	ft_printf_ptr(unsigned long long ptr, t_state *ret)
{
	if (ptr == 0)
	{
		ft_printf_str(NULL_PTR, ret);
	}
	else
	{
		ft_printf_str("0x", ret);
		ft_printf_hex_lowercase(ptr, ret);
	}
}

void	ft_printf_hex_lowercase(unsigned long long ptr, t_state *ret)
{
	if (ptr > 15)
	{
		ft_printf_hex_lowercase(ptr / 16, ret);
		ft_printf_hex_lowercase(ptr % 16, ret);
	}
	else
	{
		if (ptr < 10)
			ft_printf_char(ptr + '0', ret);
		else
			ft_printf_char(ptr - 10 + 'a', ret);
	}
}

void	ft_printf_hex_uppercase(unsigned int nbr, t_state *ret)
{
	if (nbr > 15)
	{
		ft_printf_hex_uppercase(nbr / 16, ret);
		ft_printf_hex_uppercase(nbr % 16, ret);
	}
	else
	{
		if (nbr < 10)
			ft_printf_char(nbr + '0', ret);
		else
			ft_printf_char(nbr - 10 + 'A', ret);
	}
}
