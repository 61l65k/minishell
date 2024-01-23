/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_m_libft.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 19:15:52 by apyykone          #+#    #+#             */
/*   Updated: 2023/11/17 16:00:58 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# ifdef __linux__
#  include <stdint.h>
# endif
# include <limits.h>
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *s, ...);
int		ft_m_putstr(char *s);
int		ft_m_putchar(int c);
int		ft_m_putnbr(int nbr);
int		ft_m_puthex(unsigned int dec, char c);
int		ft_m_putpointer(size_t ptr);
int		ft_m_writeback(char *hexbuff, int start, int flag);
int		ft_m_put_uint(unsigned int dec);
#endif
