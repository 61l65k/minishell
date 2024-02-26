/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 11:43:34 by ttakala           #+#    #+#             */
/*   Updated: 2024/01/05 14:58:52 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNAL_H
# define FT_PRINTF_INTERNAL_H

typedef struct s_ret
{
	int	print_len;
	int	fail_state;
	int	fd;
}	t_ret;

void	ft_printf_char(int c, t_ret *ret);
void	ft_printf_str(char *str, t_ret *ret);
void	ft_printf_ptr(unsigned long long ptr, t_ret *ret);
void	ft_printf_hex_lowercase(unsigned long long ptr, t_ret *ret);
void	ft_printf_hex_uppercase(unsigned int nbr, t_ret *ret);
void	ft_printf_nbr(long long nbr, t_ret *ret);
void	ft_printf_nbr_unsigned(unsigned int nbr, t_ret *ret);

#endif