/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 11:43:34 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 17:49:12 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNAL_H
# define FT_PRINTF_INTERNAL_H

# include <stddef.h>

# ifdef __linux__
#  define NULL_PTR "(nil)"
# else
#  define NULL_PTR "0x0"
# endif

typedef struct s_buffer
{
	char	arr[4096];
	size_t	len;
}	t_buffer;

typedef struct s_state
{
	int			print_len;
	int			bytes_written;
	int			fd;
	t_buffer	buffer;
}	t_state;

void	*ft_memset(void *b, int c, size_t len);
void	ft_printf_char(int c, t_state *ret);
void	ft_printf_str(char *str, t_state *ret);
void	ft_printf_ptr(unsigned long long ptr, t_state *ret);
void	ft_printf_hex_lowercase(unsigned long long ptr, t_state *ret);
void	ft_printf_hex_uppercase(unsigned int nbr, t_state *ret);
void	ft_printf_nbr(long long nbr, t_state *ret);
void	ft_printf_nbr_unsigned(unsigned int nbr, t_state *ret);

#endif