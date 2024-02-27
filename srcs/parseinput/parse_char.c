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
 * h->i += 1 to skip the escape character.
 */
static void	handle_escape_sequence(t_parsehelper *h, const char *input_string)
{
	h->i++;
	if (input_string[h->i] == 'n')
		h->curr_cmd[h->j++] = '\n';
	else if (input_string[h->i] == 't')
		h->curr_cmd[h->j++] = '\t';
	else if (input_string[h->i] == 'r')
		h->curr_cmd[h->j++] = '\r';
	else if (input_string[h->i] == '\\')
		h->curr_cmd[h->j++] = '\\';
	else if (input_string[h->i] == '"')
		h->curr_cmd[h->j++] = '\"';
	else if (input_string[h->i] == '\'')
		h->curr_cmd[h->j++] = '\'';
	else
		h->curr_cmd[h->j++] = input_string[h->i];
}

/**
 * @brief Turns The env variables to their values. Reallocates new memory
 * if needed for the command. h->i += 1 to skip the $ character.
 */
static void	handle_env_variable(t_parsehelper *h, t_shellstate *state)
{
	t_envhelper	eh;

	ft_memset(&eh, 0, sizeof(t_envhelper));
	eh.var_name_len = ft_envlen(state->input_string + (h->i += 1));
	eh.var_name = ft_strndup(state->input_string + h->i, eh.var_name_len);
	if (!eh.var_name)
		return (ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE));
	h->i += eh.var_name_len - 1;
	eh.var_value = getenv(eh.var_name);
	if (eh.var_value)
	{
		eh.value_len = ft_strlen(eh.var_value);
		eh.req_size = ft_strlen(h->curr_cmd) + eh.value_len + 1;
		ensure_memory_for_cmd(h, state, eh.req_size);
		ft_strncat(h->curr_cmd, eh.var_value, eh.value_len);
		h->j = ft_strlen(h->curr_cmd);
	}
	free(eh.var_name);
}

/**
 * @brief Parses the characer from input string.
 * & Handles single and double quotes,
	escape sequences and environment variables.
 *
 */
void	parse_cmd_char(t_parsehelper *h, t_shellstate *state)
{
	t_charflags	flags;
	char		c;

	c = state->input_string[h->i];
	if (init_char_flags(&flags, &state->input_string[h->i], h) == IS_QUOTE)
		return ;
	if (h->in_single_quote || h->in_double_quote)
	{
		if (flags.is_escaped)
			handle_escape_sequence(h, state->input_string);
		else if (h->in_double_quote && flags.is_env_var)
			handle_env_variable(h, state);
		else
			h->curr_cmd[h->j++] = c;
	}
	else if (flags.is_pipe)
	{
		h->curr_cmd[h->j] = '\0';
		h->commands[h->command_index++] = ft_strdup(h->curr_cmd);
		h->j = 0;
	}
	else if (flags.is_env_var)
		handle_env_variable(h, state);
	else
	{
		ensure_memory_for_cmd(h, state, 1);
		h->curr_cmd[h->j++] = c;
	}
	h->curr_cmd[h->j] = '\0';
}
