/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parentheses.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 19:36:39 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/10 19:36:40 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	adjust_index(const char *str, int i, bool adjust_next)
{
	if (adjust_next)
	{
		while (str[i] && isspace(str[i]))
			i++;
	}
	else
	{
		while (i >= 0 && isspace(str[i]))
			i--;
		if (i > 0 && (str[i] == '&' || (str[i] == '|' && str[i - 1] == '|')))
			i--;
	}
	return (i);
}

int	check_parentheses(t_operatorhelper *op, t_shellstate *s)
{
	const char	*current_char = &s->input_string[op->i];
	const int	prev_index = adjust_index(s->input_string, op->i - 1, false);
	const int	next_index = adjust_index(s->input_string, op->i + 1, true);

	if (*current_char == '(')
	{
		if (op->i != 0 && prev_index >= 0 && check_for_op(op, s,
				prev_index) == OP_NONE && *(current_char - 1) != '(')
			return (print_syntax_err("(", NULL), FAILURE);
		op->paren_depth++;
	}
	else if (*current_char == ')')
	{
		if (op->paren_depth <= 0)
			return (print_syntax_err(")", NULL), FAILURE);
		if (s->input_string[next_index] != '\0' && check_for_op(op, s,
				next_index) == OP_NONE && *(current_char + 1) != ')')
			return (print_syntax_err(")", NULL), FAILURE);
		op->paren_depth--;
	}
	return (SUCCESS);
}
