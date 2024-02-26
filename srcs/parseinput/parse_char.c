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
 * @brief Handles the environment variable in the input string.
 */
static void	handle_env_variable(t_parsehelper *h, const char *input_string)
{
	char	var_name[MAX_VAR_LENGTH];
	int		var_name_index;
	char	*var_value;

	var_name_index = 0;
	h->i++;
	while (ft_isenv_var(input_string[h->i]) && var_name_index < MAX_VAR_LENGTH
		- 1)
	{
		var_name[var_name_index++] = input_string[h->i++];
	}
	var_name[var_name_index] = '\0';
	if ((var_value = getenv(var_name)))
	{
		while (*var_value)
			h->current_command[h->j++] = *var_value++;
	}
	h->i--;
}

/**
 * @brief Characters inside duoble quoted strings are handled here.
 */
static void	handle_double_quote_char(t_parsehelper *h, const char *input_string)
{
	if (input_string[h->i] == '\\')
		handle_escape_sequence(h, input_string);
	else if (input_string[h->i] == '$')
		handle_env_variable(h, input_string);
	else
		h->current_command[h->j++] = input_string[h->i];
}

void	parse_cmd_char(t_parsehelper *h, const char *input_string)
{
	if ((input_string[h->i] == '\'' && !h->in_double_quote)
			|| (input_string[h->i] == '"' && !h->in_single_quote))
	{
		h->in_single_quote ^= (input_string[h->i] == '\'');
		h->in_double_quote ^= (input_string[h->i] == '"');
	}
	else if (h->in_single_quote)
	{
		if (input_string[h->i] == '\\')
			handle_escape_sequence(h, input_string);
		else
			h->current_command[h->j++] = input_string[h->i];
	}
	else if (h->in_double_quote)
		handle_double_quote_char(h, input_string);
	else if (input_string[h->i] == '|' && !h->in_single_quote
		&& !h->in_double_quote)
	{
		h->current_command[h->j] = '\0';
		h->commands[h->command_index++] = ft_strdup(h->current_command);
		h->j = 0;
	}
	else if (input_string[h->i] == '$')
		handle_env_variable(h, input_string);
	else
		h->current_command[h->j++] = input_string[h->i];
}
