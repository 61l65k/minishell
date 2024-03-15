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
#include "miniutils.h"
#include <stdbool.h>

void	append_operator(t_operators operator_type, t_shellstate *s,
		t_parsehelper *ph)
{
	if (operator_type != OP_NONE)
	{
		ph->i += (operator_type == OP_OR || operator_type == OP_AND);
		ensure_mem_for_buff(ph, s, 1, s->operators);
		s->operators[ph->j++] = operator_type;
		ph->command_count++;
	}
}

t_operators	check_for_op(t_parsehelper *ph, t_shellstate *s, int index)
{
	int	i;

	i = ph->i;
	if (index != -1)
		i = index;
	if (ft_strncmp(s->input_string + i, "&&", 2) == 0)
		return (OP_AND);
	if (ft_strncmp(s->input_string + i, "||", 2) == 0)
		return (OP_OR);
	if (s->input_string[i] == '|')
		return (OP_PIPE);
	return (OP_NONE);
}

void	ensure_mem_for_buff(t_parsehelper *ph, t_shellstate *s,
		size_t additional_length, void *buff)
{
	size_t	required_size;
	size_t	new_size;
	void	*new_command;

	required_size = ph->j + additional_length + 1;
	if (required_size > ph->alloc_size)
	{
		new_size = required_size * 2;
		new_command = ft_realloc(buff, ph->alloc_size, new_size);
		if (!new_command)
			return (ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE));
		ph->curr_cmd = new_command;
		ph->alloc_size = new_size;
	}
}

void	init_char_flags(int *flags, char *c, t_parsehelper *h)
{
	*flags = 0;
	if ((*c == '\'' && !h->in_double_quote) || (*c == '"'
			&& !h->in_single_quote))
	{
		h->in_single_quote ^= (*c == '\'');
		h->in_double_quote ^= (*c == '"');
		*flags |= (1 << QUOTE_BIT);
	}
	*flags |= (*c == '\\') << ESCAPED_BIT;
	*flags |= (*c == '$') << ENVVAR_BIT;
	if (!h->in_single_quote && !h->in_double_quote)
	{
		*flags |= (*c == '~' && *(c - 1) == ' ' && (*(c + 1) == ' ' || *(c
						+ 1) == '\0' || *(c + 1) == '/')) << TILDA_BIT;
		*flags |= (*c == '|' && *(c + 1) != '|') << PIPE_BIT;
		*flags |= (*c == '&' && *(c + 1) == '&') << AND_BIT;
		*flags |= (*c == '|' && *(c + 1) == '|') << OR_BIT;
		*flags |= ((*c == '>' && (*(c + 1) != '>')) || (*c == '<' && (*(c
							+ 1) != '<')) || (*c == '>' && *(c + 1) == '>')
				|| (*c == '<' && *(c + 1) == '<')) << REDIR_BIT;
	}
}
