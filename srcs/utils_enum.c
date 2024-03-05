/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_enum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 09:19:38 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/05 10:29:10 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*op_to_str(t_operators op)
{
	static const char	*names[OP_OPERATOR_COUNT] = {
	[OP_NONE] = "",
	[OP_AND] = "&&",
	[OP_OR] = "||",
	[OP_HEREDOC] = "<<",
	[OP_APPEND] = ">>",
	[OP_PIPE] = "|",
	[OP_REDIRECT_OUT] = ">",
	[OP_REDIRECT_IN] = "<"};

	return (names[op]);
}
