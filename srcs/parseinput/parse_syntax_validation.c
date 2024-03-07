/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:22:30 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 13:43:12 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

bool	is_valid_cmd_count(t_shellstate *state)
{
	int	i;

	i = 0;
	while (state->parsed_cmds[i] && state->operators[i])
	{
		if (state->operators[i] == OP_NONE)
			break ;
		i++;
	}
	if (state->parsed_cmds[i] && state->parsed_cmds[i + 1])
	{
		ft_fprintf(2, "syntax error near unexpected token `%s'\n",
			state->parsed_cmds[i + 1]->content);
		return (false);
	}
	if (state->operators[i] != OP_NONE)
	{
		ft_fprintf(2, "syntax error near unexpected token `%s'\n",
			op_to_str(state->operators[i]));
		return (false);
	}
	return (true);
}
