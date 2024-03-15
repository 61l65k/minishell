/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_char_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:29:24 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/14 11:29:26 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	adjust_index_for_parentheses_check(const char *str, int i, bool adjust_next)
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
	const char	*current_char = &s->input_string[ph->i];
	const int	prev_index = adjust_index_for_parentheses_check(s->input_string,
			ph->i - 1, false);
	const int	next_index = adjust_index_for_parentheses_check(s->input_string,
			ph->i + 1, true);

	if (*current_char == '(')
	{
		if (ph->i != 0 && prev_index >= 0 && check_for_op(ph, s,
				prev_index) == OP_NONE && *(current_char - 1) != '(')
			return (print_syntax_err("(", NULL), FAILURE);
		*paren_depth += 1;
	}
	else if (*current_char == ')')
	{
		if (*paren_depth <= 0)
			return (print_syntax_err(")", NULL), FAILURE);
		if (s->input_string[next_index] != '\0' && check_for_op(ph, s,
				next_index) == OP_NONE && *(current_char + 1) != ')')
			return (print_syntax_err(")", NULL), FAILURE);
		*paren_depth -= 1;
	}
	return (SUCCESS);
}

void	handle_tilda(t_parsehelper *h, t_shellstate *state)
{
	const char	*tilda = ft_getenv("HOME", state->envp);
	const int	til_len = ft_strlen(tilda);

	if (tilda)
	{
		ensure_mem_for_buff(h, state, ft_strlen(h->curr_cmd) \
							+ til_len + 1, false);
		ft_strncat(h->curr_cmd, tilda, til_len);
		h->j = ft_strlen(h->curr_cmd);
	}
}

int	ft_checkdollar(t_shellstate *s, t_parsehelper *h)
{
	const char	*c = &s->input_string[h->i];

	if (!h->in_single_quote && !h->in_double_quote && (*(c + 1) == ' ' || *(c
				+ 1) == '\0'))
	{
		ensure_mem_for_buff(h, s, 1, false);
		h->curr_cmd[h->j++] = *c;
		return (1);
	}
	else if ((h->in_single_quote || h->in_double_quote) && (*(c + 1) == '\''
			|| *(c + 1) == '"' || *(c + 1) == ' ' || *(c + 1) == '$'))
	{
		ensure_mem_for_buff(h, s, 1, false);
		h->curr_cmd[h->j++] = *c;
		return (1);
	}
	return (0);
}
