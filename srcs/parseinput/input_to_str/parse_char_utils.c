/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 19:12:29 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 19:12:30 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ensure_mem_cpy_op(t_operatorhelper *op, t_operators operator_type,
		t_shellstate *s)
{
	size_t		new_capacity;
	t_operators	*new_operators;

	if (operator_type == OP_NONE)
		return ;
	if (operator_type == OP_HEREDOC || operator_type == OP_APPEND
		|| operator_type == OP_OR || operator_type == OP_AND)
		op->i++;
	if ((size_t)op->cmd_count >= op->operators_capacity)
	{
		new_capacity = op->operators_capacity * 2;
		new_operators = ft_realloc(op->ops, op->operators_capacity
				* sizeof(t_operators), new_capacity * sizeof(t_operators));
		if (!new_operators)
			ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
		op->ops = new_operators;
		op->operators_capacity = new_capacity;
	}
	op->ops[op->ops_i++] = operator_type;
	op->cmd_count++;
}

t_operators	check_for_op(t_operatorhelper *op, t_shellstate *state, int index)
{
	int	i;

	i = op->i;
	if (index != -1)
		i = index;
	if (ft_strncmp(state->input_string + i, "&&", 2) == 0)
		return (OP_AND);
	if (ft_strncmp(state->input_string + i, "||", 2) == 0)
		return (OP_OR);
	if (state->input_string[i] == '|')
		return (OP_PIPE);
	return (OP_NONE);
}

void	ensure_mem_for_buff(t_parsehelper *h, t_shellstate *s,
		size_t additional_length, bool op_buff)
{
	size_t	required_size;
	size_t	new_size;
	void	*new_command;
	void	*buff;

	buff = h->curr_cmd;
	if (op_buff)
		buff = s->operators;
	required_size = h->j + additional_length + 1;
	if (required_size > h->alloc_size)
	{
		new_size = required_size * 2;
		new_command = ft_realloc(buff, h->alloc_size, new_size);
		if (!new_command)
			ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
		h->curr_cmd = new_command;
		h->alloc_size = new_size;
	}
}

void	init_char_flags(t_envhelper *eh, char *c, t_parsehelper *h)
{
	*eh = (t_envhelper){0};
	if ((*c == '\'' && !h->in_double_quote) || (*c == '"'
			&& !h->in_single_quote))
	{
		h->in_single_quote ^= (*c == '\'');
		h->in_double_quote ^= (*c == '"');
		eh->flags |= (1 << QUOTE_BIT);
	}
	eh->flags |= (*c == '\\') << ESCAPED_BIT;
	eh->flags |= (*c == '$') << ENVVAR_BIT;
	if (!h->in_single_quote && !h->in_double_quote)
	{
		eh->flags |= (*c == '~' && *(c - 1) == ' ' && (*(c + 1) == ' ' || *(c
						+ 1) == '\0' || *(c + 1) == '/')) << TILDA_BIT;
		eh->flags |= (*c == '|' && *(c + 1) != '|') << PIPE_BIT;
		eh->flags |= (*c == '&' && *(c + 1) == '&') << AND_BIT;
		eh->flags |= (*c == '|' && *(c + 1) == '|') << OR_BIT;
		eh->flags |= ((*c == '>' && (*(c + 1) != '>')) || (*c == '<' && (*(c
							+ 1) != '<')) || (*c == '>' && *(c + 1) == '>')
				|| (*c == '<' && *(c + 1) == '<')) << REDIR_BIT;
	}
}
