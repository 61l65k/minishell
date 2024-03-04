/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseinput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 17:03:46 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"

/**
 * @brief Check if the the current character is operator.
 */
static t_operators	check_for_op(t_operatorhelper *op, t_shellstate *state)
{
	if (ft_strncmp(state->input_string + op->i, "&&", 2) == 0)
		return (OP_AND);
	if (ft_strncmp(state->input_string + op->i, "||", 2) == 0)
		return (OP_OR);
	if (ft_strncmp(state->input_string + op->i, "<<", 2) == 0)
		return (OP_HEREDOC);
	if (ft_strncmp(state->input_string + op->i, ">>", 2) == 0)
		return (OP_APPEND);
	if (state->input_string[op->i] == '|')
		return (OP_PIPE);
	if (state->input_string[op->i] == '>')
		return (OP_REDIRECT_OUT);
	if (state->input_string[op->i] == '<')
		return (OP_REDIRECT_IN);
	return (OP_NONE);
}

/**
 * @brief ensures that there is enough memory for the operators array.
 */
static void	ensure_mem_cpy_op(t_operatorhelper *op, t_operators operator_type,
		t_shellstate *state)
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
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		op->ops = new_operators;
		op->operators_capacity = new_capacity;
	}
	op->ops[op->ops_i++] = operator_type;
	op->cmd_count++;
}

/**
 * @brief Counts the amount of commands in the input string.
 * & Returns the amount of commands.
 */
static int	count_op_cmds(t_shellstate *state, int *cmd_count)
{
	t_operatorhelper	op;

	ft_memset(&op, 0, sizeof(t_operatorhelper));
	op.operators_capacity = 100;
	op.cmd_count = 1;
	op.ops = ft_calloc(op.operators_capacity, sizeof(t_operators));
	if (!op.ops)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[op.i])
	{
		if (state->input_string[op.i] == '\'' && !op.in_double_quote)
			op.in_single_quote = !op.in_single_quote;
		else if (state->input_string[op.i] == '"' && !op.in_single_quote)
			op.in_double_quote = !op.in_double_quote;
		if (!op.in_single_quote && !op.in_double_quote)
			ensure_mem_cpy_op(&op, check_for_op(&op, state), state);
		op.i++;
	}
	state->operators = ft_realloc(state->operators, state->operator_count
			* sizeof(t_operators), op.cmd_count * sizeof(t_operators));
	ft_memcpy(state->operators, op.ops, op.cmd_count * sizeof(t_operators));
	state->operator_count = op.cmd_count - 1;
	*cmd_count = op.cmd_count;
	return (free(op.ops), op.in_single_quote || op.in_double_quote);
}

/**
 * @brief Splits the input string into commands delimeted by operators.
 * & Returns a NULL-terminated array of strings.
 */
static char	**split_cmds(t_shellstate *state, t_parsehelper *h)
{
	h->i = -1;
	h->alloc_size = ft_strlen(state->input_string) + 1;
	if (count_op_cmds(state, &h->command_count) == QUOTE_ERROR)
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
 * & Returns 0 if successful, otherwise 1.
 */
int	ft_parseinput(t_shellstate *s)
{
	int				i;
	char			*trimmed_command;
	t_parsehelper	h;

	ft_memset(&h, 0, sizeof(t_parsehelper));
	i = -1;
	s->parsed_args = split_cmds(s, &h);
	if (!s->parsed_args)
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), EXIT_FAILURE);
	while (s->parsed_args[s->cmd_count])
		s->cmd_count++;
	s->operator_count = s->cmd_count - 1;
	if (!s->parsed_args)
		ft_free_exit(s, ERR_PROCESTRING, EXIT_FAILURE);
	while (++i < s->cmd_count)
	{
		trimmed_command = trim_command(s->parsed_args[i]);
		if (trimmed_command != s->parsed_args[i])
			free(s->parsed_args[i]);
		s->parsed_args[i] = trimmed_command;
	}
	return (EXIT_SUCCESS);
}
