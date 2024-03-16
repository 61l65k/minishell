/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parentheses.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:06:41 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/16 21:06:42 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	adjust_index_for_parentheses_check(const char *str, int i,
		bool adjust_next)
{
	if (adjust_next)
	{
		while (str[i] && str[i] == ' ')
			i++;
	}
	else
	{
		while (i >= 0 && str[i] == ' ')
			i--;
		if (i > 0 && (str[i] == '&' || (str[i] == '|' && str[i - 1] == '|')))
			i--;
	}
	return (i);
}

int	check_parentheses(int *paren_depth, t_shellstate *s, t_parsehelper *ph)
{
	const char		*input = s->input_string;
	const int		i = ph->i;
	const int		prev_index = adjust_index_for_parentheses_check(input,
			i - 1, false);
	const int		next_index = adjust_index_for_parentheses_check(input,
			i + 1, true);

	if (input[i] == '(')
	{
		if (i != 0 && input[prev_index] != '(' && input[prev_index] != ')'
			&& check_for_op(ph, s, prev_index) == OP_NONE)
			return (print_syntax_err("(", NULL), FAILURE);
		(*paren_depth)++;
	}
	else if (input[i] == ')')
	{
		if (*paren_depth <= 0)
			return (print_syntax_err(")", NULL), FAILURE);
		if (input[next_index] != '\0' && input[next_index] != '('
			&& input[next_index] != ')' && check_for_op(ph, s,
				next_index) == OP_NONE)
			return (print_syntax_err(")", NULL), FAILURE);
		(*paren_depth)--;
	}
	return (SUCCESS);
}
