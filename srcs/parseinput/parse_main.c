/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 14:43:27 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <stdlib.h>

static int	validation_loop(t_shellstate *s, t_operatorhelper *op)
{
	while (s->input_string[op->i])
	{
		if (s->input_string[op->i] == '\'' && !op->in_double_quote)
			op->in_single_quote = !op->in_single_quote;
		else if (s->input_string[op->i] == '"' && !op->in_single_quote)
			op->in_double_quote = !op->in_double_quote;
		if (!op->in_single_quote && !op->in_double_quote)
		{
			if (check_parentheses(op, s) == FAILURE)
				return (FAILURE);
			ensure_mem_cpy_op(op, check_for_op(op, s, -1), s);
		}
		op->i++;
	}
	if (op->in_single_quote || op->in_double_quote)
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), FAILURE);
	if (op->paren_depth != 0)
		return (ft_putstr_fd(ERR_PARENTHESES, STDERR_FILENO), FAILURE);
	return (SUCCESS);
}

/**
 * @brief Counts the amount of commands in the input string.
 * & Returns the amount of commands.
 */
static int	count_op_cmds(t_shellstate *s, int *cmd_count)
{
	t_operatorhelper	op;

	op = (t_operatorhelper){0};
	op.operators_capacity = 100;
	op.cmd_count = 1;
	op.ops = ft_calloc(op.operators_capacity, sizeof(t_operators));
	if (!op.ops)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	if (validation_loop(s, &op) == FAILURE)
		return (free(op.ops), FAILURE);
	s->operators = ft_realloc(s->operators, s->operator_count
			* sizeof(t_operators), op.cmd_count * sizeof(t_operators));
	ft_memcpy(s->operators, op.ops, op.cmd_count * sizeof(t_operators));
	s->operator_count = op.cmd_count - 1;
	*cmd_count = op.cmd_count;
	return (free(op.ops), SUCCESS);
}

/**
 * @brief Splits the input string into commands delimeted by operators.
 * & Returns a NULL-terminated array of strings.
 */
static char	**split_cmds(t_shellstate *state, t_parsehelper *h)
{
	h->i = -1;
	h->alloc_size = ft_strlen(state->input_string) + 1;
	if (count_op_cmds(state, &h->command_count) != SUCCESS)
		return (NULL);
	h->commands = ft_calloc(h->command_count + 1, sizeof(char *));
	h->curr_cmd = ft_calloc(h->alloc_size, 1);
	if (!h->commands || !h->curr_cmd)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[++h->i] != '\0')
		parse_cmd_char(h, state);
	h->curr_cmd[h->j] = '\0';
	if (h->j > 0 && h->command_index < h->command_count)
	{
		h->commands[h->command_index++] = ft_strdup(h->curr_cmd);
		if (!h->commands[h->command_index - 1])
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	}
	if (h->command_index < h->command_count)
		h->commands[h->command_index] = NULL;
	return (free(h->curr_cmd), h->commands);
}

/**
 * @brief Top function for starting parsing the input string.
 * & Returns 0 if successful, returns non zero on errors.
 */
int	ft_parseinput(t_shellstate *s)
{
	int				i;
	t_parsehelper	h;

	h = (t_parsehelper){0};
	s->parsed_args = split_cmds(s, &h);
	if (!s->parsed_args)
		return (FAILURE);
	while (s->parsed_args[s->cmd_count])
		s->cmd_count++;
	s->operator_count = s->cmd_count - 1;
	s->parsed_cmds = ft_calloc(s->cmd_count + 1, sizeof(t_list *));
	if (!s->parsed_cmds)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	i = -1;
	while (++i < s->cmd_count)
	{
		s->parsed_cmds[i] = str_to_lst(s->parsed_args[i], &h);
		if (s->parsed_cmds[i] == NULL)
		{
			if (!h.ambigious_error)
				print_syntax_err(op_to_str(s->operators[i]), s->parsed_args[i]);
			return (free_and_null_str_array(&s->parsed_args), 2);
		}
	}
	free_and_null_str_array(&s->parsed_args);
	return (EXIT_SUCCESS);
}
