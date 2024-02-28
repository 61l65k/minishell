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

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <unistd.h>

/**
 * @brief Counts the amount of commands in the input string.
 * & Returns the amount of commands.
 */
static int	count_operators_cmds(t_shellstate *state)
{
	t_parsehelper	h;
	char			*operators;
	size_t			operators_size;
	size_t			operators_capacity;
	char			*input_string;
	const char		*op = NULL;
	size_t			op_len;

	operators_size = 0;
	operators_capacity = 10;
	input_string = state->input_string;
	input_string = state->input_string;
	ft_memset(&h, 0, sizeof(t_parsehelper));
	operators = ft_calloc(10, (h.alloc_size += 10));
	if (!operators)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	h.command_count = 1;
	while (input_string[h.i])
	{
		if (input_string[h.i] == '\'' && !h.in_double_quote)
			h.in_single_quote = !h.in_single_quote;
		else if (input_string[h.i] == '"' && !h.in_single_quote)
			h.in_double_quote = !h.in_double_quote;
		else if (!h.in_single_quote && !h.in_double_quote)
		{
			op = NULL;
			if (input_string[h.i] == '|')
			{
				op = "|?";
				if (input_string[h.i + 1] == '|')
				{
					h.i++;
					op = "||?";
				}
				h.command_count++;
			}
			else if (input_string[h.i] == '&' && input_string[h.i + 1] == '&')
			{
				op = "&&?";
				h.i++;
				h.command_count++;
			}
			else if (input_string[h.i] == '>')
			{
				op = ">?";
				if (input_string[h.i + 1] == '>')
				{
					op = ">>?";
					h.i++;
				}
				h.command_count++;
			}
			else if (input_string[h.i] == '<')
			{
				op = "<?";
				if (input_string[h.i + 1] == '<')
				{
					op = "<<?";
					h.i++;
				}
				h.command_count++;
			}
			if (op)
			{
				op_len = ft_strlen(op);
				if (operators_size + op_len + 1 > operators_capacity)
				{
					operators_capacity *= 2;
					operators = ft_realloc(operators, operators_size,
							operators_capacity);
					if (!operators)
						ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
				}
				ft_strlcpy(operators + operators_size, op, operators_capacity
					- operators_size);
				operators_size += op_len;
				h.command_count++;
			}
		}
		h.i++;
	}
	if (operators)
	{
		state->operators = ft_split(operators, '?');
		if (!state->operators)
		{
			free(operators);
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		}
	}
	return (free(operators), h.command_count);
}

/**
 * @brief Parses the command character by character.
 */
static char	**split_cmds(t_shellstate *state)
{
	t_parsehelper	h;

	ft_memset(&h, 0, sizeof(t_parsehelper));
	h.alloc_size = ft_strlen(state->input_string) + 1;
	h.command_count = count_operators_cmds(state);
	h.commands = ft_calloc(h.command_count + 1, sizeof(char *));
	h.curr_cmd = ft_calloc(h.alloc_size, 1);
	if (!h.commands || !h.curr_cmd)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[h.i] != '\0')
	{
		parse_cmd_char(&h, state);
		h.i++;
	}
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
