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

#include "minishell.h"

static int	check_for_op(t_operatorhelper *op, t_shellstate *state)
{
	op->op[0] = '\0';
	if (ft_strncmp(state->input_string + op->i, "&&", 2) == 0
		|| ft_strncmp(state->input_string + op->i, "||", 2) == 0
		|| ft_strncmp(state->input_string + op->i, "<<", 2) == 0
		|| ft_strncmp(state->input_string + op->i, ">>", 2) == 0)
	{
		ft_strlcpy(op->op, state->input_string + op->i, 3);
		op->op[2] = '?';
		op->op[3] = '\0';
		op->i++;
		return (FOUNDOP);
	}
	else if (state->input_string[op->i] == '|'
		|| state->input_string[op->i] == '>'
		|| state->input_string[op->i] == '<')
	{
		op->op[0] = state->input_string[op->i];
		op->op[1] = '?';
		op->op[2] = '\0';
		op->i++;
		return (FOUNDOP);
	}
	return (SUCCESS);
}

static void	ensure_mem_cpy_op(t_operatorhelper *op, t_shellstate *state)
{
	char	*new_operators;

	op->op_len = ft_strlen(op->op);
	if (op->operators_size + op->op_len + 1 > op->operators_capacity)
	{
		op->operators_capacity *= 2;
		new_operators = ft_realloc(op->operators, op->operators_size,
				op->operators_capacity);
		if (!new_operators)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		op->operators = new_operators;
	}
	ft_strlcpy(op->operators + op->operators_size, op->op,
		op->operators_capacity - op->operators_size);
	op->operators_size += op->op_len;
	op->command_count++;
}

/**
 * @brief Counts the amount of commands in the input string.
 * & Returns the amount of commands.
 */
static int	count_op_cmds(t_shellstate *state)
{
	t_operatorhelper	op;

	ft_memset(&op, 0, sizeof(t_operatorhelper));
	op.operators = ft_calloc((op.operators_capacity += 100), 1);
	op.command_count = 1;
	if (!op.operators)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[op.i])
	{
		if (state->input_string[op.i] == '\'' && !op.in_double_quote)
			op.in_single_quote = !op.in_single_quote;
		else if (state->input_string[op.i] == '"' && !op.in_single_quote)
			op.in_double_quote = !op.in_double_quote;
		else if (!op.in_single_quote && !op.in_double_quote && check_for_op(&op,
				state) == FOUNDOP)
			ensure_mem_cpy_op(&op, state);
		op.i++;
	}
	if (op.operators)
	{
		state->operators = ft_split(op.operators, '?');
		if (!state->operators)
			return (free(op.operators), ft_free_exit(state, ERR_MALLOC, 1), 1);
	}
	return (free(op.operators), op.command_count);
}

/**
 * @brief Splits the input string into commands delimeted by operators.
 * & Returns a NULL-terminated array of strings.
 */
static char	**split_cmds(t_shellstate *state)
{
	t_parsehelper	h;

	ft_memset(&h, 0, sizeof(t_parsehelper));
	h.i = -1;
	h.alloc_size = ft_strlen(state->input_string) + 1;
	h.command_count = count_op_cmds(state);
	h.commands = ft_calloc(h.command_count + 1, sizeof(char *));
	h.curr_cmd = ft_calloc(h.alloc_size, 1);
	if (!h.commands || !h.curr_cmd)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[++h.i] != '\0')
		parse_cmd_char(&h, state);
	if (h.in_double_quote || h.in_single_quote)
		return (free(h.curr_cmd), free(h.commands), NULL);
	h.curr_cmd[h.j] = '\0';
	if (h.j > 0 && h.command_index < h.command_count)
	{
		h.commands[h.command_index++] = ft_strdup(h.curr_cmd);
		if (!h.commands[h.command_index - 1])
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	}
	if (h.command_index < h.command_count)
		h.commands[h.command_index] = NULL;
	return (free(h.curr_cmd), h.commands);
}

/**
 * @brief Top function for starting parsing the input string.
 * & Returns 0 if successful, otherwise 1.
 */
int	ft_parseinput(t_shellstate *state)
{
	int		i;
	char	*trimmed_command;

	i = -1;
	state->parsed_args = split_cmds(state);
	if (!state->parsed_args)
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), EXIT_FAILURE);
	while (state->parsed_args[state->cmd_count])
		state->cmd_count++;
	state->operator_count = state->cmd_count - 1;
	if (!state->parsed_args)
		ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
	while (++i < state->cmd_count)
	{
		trimmed_command = trim_spaces(state->parsed_args[i]);
		if (trimmed_command != state->parsed_args[i])
			free(state->parsed_args[i]);
		state->parsed_args[i] = trimmed_command;
	}
	return (EXIT_SUCCESS);
}
