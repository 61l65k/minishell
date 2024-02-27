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

/**
 * @brief Counts the amount of commands in the input string.
 * & Returns the amount of commands.
 */
static int	count_commands(const char *input_string)
{
	t_parsehelper	h;

	ft_memset(&h, 0, sizeof(t_parsehelper));
	h.command_count = 1;
	while (input_string[h.i])
	{
		if (input_string[h.i] == '\'' && !h.in_double_quote)
			h.in_single_quote = !h.in_single_quote;
		else if (input_string[h.i] == '"' && !h.in_single_quote)
			h.in_double_quote = !h.in_double_quote;
		else if (!h.in_single_quote && !h.in_double_quote)
		{
			if (input_string[h.i] == '|' || input_string[h.i] == '&'
				|| input_string[h.i] == '>' || input_string[h.i] == '<')
			{
				h.command_count++;
				if (input_string[h.i + 1] == input_string[h.i])
					h.i++;
			}
		}
		h.i++;
	}
	return (h.command_count);
}

/**
 * @brief Parses the command character by character.
 */
static char	**split_cmds(t_shellstate *state)
{
	t_parsehelper	h;

	ft_memset(&h, 0, sizeof(t_parsehelper));
	h.curr_size = ft_strlen(state->input_string) + 1;
	h.command_count = count_commands(state->input_string);
	h.commands = malloc(sizeof(char *) * (h.command_count + 1));
	h.curr_cmd = malloc(ft_strlen(state->input_string) + 1);
	if (!h.commands || !h.curr_cmd)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[h.i] != '\0')
	{
		parse_cmd_char(&h, state);
		h.i++;
	}
	if (h.in_single_quote || h.in_double_quote)
	{
		free(h.curr_cmd);
		free(h.commands);
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), NULL);
	}
	if (h.j > 0)
	{
		h.curr_cmd[h.j] = '\0';
		h.commands[h.command_index++] = ft_strdup(h.curr_cmd);
		if (!h.commands[h.command_index - 1])
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	}
	free(h.curr_cmd);
	h.commands[h.command_index] = NULL;
	return (h.commands);
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
		return (EXIT_FAILURE);
	while (state->parsed_args[state->cmd_count])
		state->cmd_count++;
	if (!state->parsed_args)
		ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
	state->is_piped = (state->cmd_count > 1);
	while (++i < state->cmd_count)
	{
		trimmed_command = trim_spaces(state->parsed_args[i]);
		if (trimmed_command != state->parsed_args[i])
			free(state->parsed_args[i]);
		state->parsed_args[i] = trimmed_command;
		// printf("parsed_args[%d]: %s\n", i, state->parsed_args[i]);
	}
	return (EXIT_SUCCESS);
}
