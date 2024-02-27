/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:52 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 17:03:53 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the escape sequence in quoted strings.
 */
static void	handle_escape_sequence(t_parsehelper *h, const char *input_string)
{
	h->i++;
	if (input_string[h->i] == 'n')
		h->current_command[h->j++] = '\n';
	else if (input_string[h->i] == 't')
		h->current_command[h->j++] = '\t';
	else if (input_string[h->i] == 'r')
		h->current_command[h->j++] = '\r';
	else if (input_string[h->i] == '\\')
		h->current_command[h->j++] = '\\';
	else if (input_string[h->i] == '"')
		h->current_command[h->j++] = '\"';
	else if (input_string[h->i] == '\'')
		h->current_command[h->j++] = '\'';
	else
		h->current_command[h->j++] = input_string[h->i];
}

/**
 * @brief handles env variables and reallocating memory for the command.
 * according to the length of the env variable.
 */
static void	handle_env_variable(t_parsehelper *h, t_shellstate *state)
{
	t_envhelper	eh;
	size_t		old_size;

	ft_memset(&eh, 0, sizeof(t_envhelper));
	eh.var_name = malloc(INITIAL_VAR_NAME_SIZE);
	if (!eh.var_name)
		return (ft_putstr_fd(ERR_MALLOC, STDERR_FILENO));
	eh.var_name_size = INITIAL_VAR_NAME_SIZE;
	eh.var_name_index = 0;
	h->i++;
	while (ft_isenv_var(state->input_string[h->i]))
	{
		if (eh.var_name_index >= eh.var_name_size - 1)
		{
			eh.old_var_name_size = eh.var_name_size;
			eh.var_name_size *= 2;
			eh.new_var_name = ft_realloc(eh.var_name, eh.old_var_name_size,
					eh.var_name_size);
			if (!eh.new_var_name)
				return (free(eh.var_name), ft_free_exit(state, ERR_MALLOC, 1));
			eh.var_name = eh.new_var_name;
		}
		eh.var_name[eh.var_name_index++] = state->input_string[h->i++];
	}
	eh.var_name[eh.var_name_index] = '\0';
	eh.var_value = getenv(eh.var_name);
	if (eh.var_value)
	{
		eh.value_len = ft_strlen(eh.var_value);
		eh.required_size = strlen(h->current_command) + eh.value_len + 1;
		if (eh.required_size > h->curr_alloc_size)
		{
			old_size = h->curr_alloc_size;
			eh.new_command = ft_realloc(h->current_command, old_size,
					eh.required_size);
			if (!eh.new_command)
			{
				free(eh.var_name);
				return (ft_free_exit(state, ERR_MALLOC, 1));
			}
			h->current_command = eh.new_command;
			h->curr_alloc_size = eh.required_size;
		}
		ft_strncat(h->current_command, eh.var_value, eh.value_len);
		h->j = ft_strlen(h->current_command);
	}
	h->i--;
	free(eh.var_name);
}

/**
 * @brief Characters inside duoble quoted strings are handled here.
 */
static void	handle_double_quote_char(t_parsehelper *h, t_shellstate *state)
{
	if (state->input_string[h->i] == '\\')
		handle_escape_sequence(h, state->input_string);
	else if (state->input_string[h->i] == '$')
		handle_env_variable(h, state);
	else
		h->current_command[h->j++] = state->input_string[h->i];
}

/**
 * @brief Parses the characer from input string.
 * & Handles single and double quotes,
	escape sequences and environment variables.
 *
 */
void	parse_cmd_char(t_parsehelper *h, t_shellstate *state)
{
	char	*new_command;
	size_t	new_size;

	if ((state->input_string[h->i] == '\'' && !h->in_double_quote)
			|| (state->input_string[h->i] == '"' && !h->in_single_quote))
	{
		h->in_single_quote ^= (state->input_string[h->i] == '\'');
		h->in_double_quote ^= (state->input_string[h->i] == '"');
	}
	else if (h->in_single_quote)
	{
		if (state->input_string[h->i] == '\\')
			handle_escape_sequence(h, state->input_string);
		else
			h->current_command[h->j++] = state->input_string[h->i];
	}
	else if (h->in_double_quote)
		handle_double_quote_char(h, state);
	else if (state->input_string[h->i] == '|' && !h->in_single_quote
		&& !h->in_double_quote)
	{
		h->current_command[h->j] = '\0';
		h->commands[h->command_index++] = ft_strdup(h->current_command);
		h->j = 0;
	}
	else if (state->input_string[h->i] == '$')
		handle_env_variable(h, state);
	else
	{
		if (h->j >= h->curr_alloc_size - 1)
		{
			new_size = h->curr_alloc_size * 2;
			new_command = realloc(h->current_command, new_size);
			if (!new_command)
				ft_free_exit(state, ERR_MALLOC, 1);
			h->current_command = new_command;
			h->curr_alloc_size = new_size;
		}
		h->current_command[h->j++] = state->input_string[h->i];
		h->current_command[h->j] = '\0';
	}
}
