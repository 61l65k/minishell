/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_type.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 19:29:23 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 22:47:45 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_TYPE_H
# define IO_TYPE_H

# include "libft.h"

typedef enum e_io_type
{
	IO_NONE,
	IO_OUT_TRUNC,
	IO_OUT_APPEND,
	IO_IN_TRUNC,
	IO_IN_HEREDOC
}		t_io_type;

typedef struct s_io
{
	t_io_type	type;
	char		*filename;
}		t_io;

typedef struct s_command
{
	char		**args;
	t_vec		io_vec;
}		t_command;

#endif