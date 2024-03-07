/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 10:01:24 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 18:33:46 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <stdarg.h>
#include <unistd.h>

static void	ft_printf_conversion_handler(va_list args, char spec, t_state *ret);
static int	print_remaining_buffer(t_state *ret);

int	ft_fprintf(int output_fd, const char *format, ...)
{
	int		i;
	va_list	args;
	t_state	ret;

	ft_memset(&ret, 0, sizeof(t_state));
	ret.fd = output_fd;
	i = 0;
	va_start(args, format);
	while (format[i] && ret.bytes_written >= 0)
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
	return (print_remaining_buffer(&ret));
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	t_state	ret;

	ft_memset(&ret, 0, sizeof(t_state));
	ret.fd = 1;
	i = 0;
	va_start(args, format);
	while (format[i] && ret.bytes_written >= 0)
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
	return (print_remaining_buffer(&ret));
}

static void	ft_printf_conversion_handler(va_list args, char spec, t_state *ret)
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

void	ft_printf_char(int c, t_state *ret)
{
	if (ret->bytes_written < 0)
		return ;
	if (ret->buffer.len < sizeof(ret->buffer.arr))
	{
		ret->buffer.arr[ret->buffer.len] = c;
		ret->buffer.len++;
	}
	else
	{
		ret->bytes_written = write(ret->fd, ret->buffer.arr, ret->buffer.len);
		if (ret->bytes_written < 0)
			return ;
		ret->print_len += ret->bytes_written;
		ft_memset(ret->buffer.arr, 0, sizeof(ret->buffer.arr));
		ret->buffer.len = 0;
		ft_printf_char(c, ret);
	}
}

static int	print_remaining_buffer(t_state *ret)
{
	if (ret->bytes_written < 0)
		return (-1);
	ret->bytes_written = write(ret->fd, ret->buffer.arr, ret->buffer.len);
	if (ret->bytes_written < 0)
		return (-1);
	ret->print_len += ret->bytes_written;
	return (ret->print_len);
}
