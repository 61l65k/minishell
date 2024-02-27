/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 10:01:24 by ttakala           #+#    #+#             */
/*   Updated: 2024/01/05 15:03:37 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <stdarg.h>
#include <unistd.h>

static void	ft_printf_conversion_handler(va_list args, char spec, t_ret *ret);

int	ft_fprintf(int output_fd, const char *format, ...)
{
	int		i;
	va_list	args;
	t_ret	ret;

	ret.print_len = 0;
	ret.fail_state = 0;
	ret.fd = output_fd;
	i = 0;
	va_start(args, format);
	while (format[i] && ret.fail_state == 0)
	{
		if (format[i] == '%' && format[i + 1])
		{
			ft_printf_conversion_handler(args, format[i + 1], &ret);
			i++;
		}
		else
			ft_printf_char(format[i], &ret);
		i++;
	}
	va_end(args);
	if (ret.fail_state < 0)
		return (-1);
	return (ret.print_len);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	t_ret	ret;

	ret.print_len = 0;
	ret.fail_state = 0;
	ret.fd = 1;
	i = 0;
	va_start(args, format);
	while (format[i] && ret.fail_state == 0)
	{
		if (format[i] == '%' && format[i + 1])
		{
			ft_printf_conversion_handler(args, format[i + 1], &ret);
			i++;
		}
		else
			ft_printf_char(format[i], &ret);
		i++;
	}
	va_end(args);
	if (ret.fail_state < 0)
		return (-1);
	return (ret.print_len);
}

static void	ft_printf_conversion_handler(va_list args, char spec, t_ret *ret)
{
	if (spec == 'c')
		ft_printf_char(va_arg(args, int), ret);
	else if (spec == 's')
		ft_printf_str(va_arg(args, char *), ret);
	else if (spec == 'p')
		ft_printf_ptr(va_arg(args, unsigned long long), ret);
	else if (spec == 'd' || spec == 'i')
		ft_printf_nbr(va_arg(args, int), ret);
	else if (spec == 'u')
		ft_printf_nbr_unsigned(va_arg(args, unsigned int), ret);
	else if (spec == 'x')
		ft_printf_hex_lowercase(va_arg(args, unsigned int), ret);
	else if (spec == 'X')
		ft_printf_hex_uppercase(va_arg(args, unsigned int), ret);
	else if (spec == '%')
		ft_printf_char('%', ret);
}

void	ft_printf_char(int c, t_ret *ret)
{
	if (ret->fail_state < 0)
		return ;
	if (write(ret->fd, &c, 1) < 0)
		ret->fail_state = -1;
	else
		ret->print_len++;
}

void	ft_printf_str(char *str, t_ret *ret)
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
