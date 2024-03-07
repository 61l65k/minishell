/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_enum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 09:19:38 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 10:15:34 by ttakala          ###   ########.fr       */
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

t_operators	str_to_op(const char *str)
{
	if (ft_strcmp(str, "&&") == 0)
		return (OP_AND);
	else if (ft_strcmp(str, "||") == 0)
		return (OP_OR);
	else if (ft_strcmp(str, "<<") == 0)
		return (OP_HEREDOC);
	else if (ft_strcmp(str, ">>") == 0)
		return (OP_APPEND);
	else if (ft_strcmp(str, "|") == 0)
		return (OP_PIPE);
	else if (ft_strcmp(str, ">") == 0)
		return (OP_REDIRECT_OUT);
	else if (ft_strcmp(str, "<") == 0)
		return (OP_REDIRECT_IN);
	return (OP_NONE);
}
