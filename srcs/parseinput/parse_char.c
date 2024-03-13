/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:52 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 13:12:22 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "miniutils.h"

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
static void	handle_env_variable(t_parsehelper *h, t_shellstate *state,
		t_envhelper *eh)
{
	h->i += 1;
	if (state->input_string[h->i] == '?')
	{
		eh->var_value = ft_itoa(state->last_exit_status);
		if (!eh->var_value)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		eh->free_var_value = true;
	}
	else
	{
		eh->var_name_len = ft_envlen(state->input_string + h->i);
		eh->var_name = ft_strndup(state->input_string + h->i, eh->var_name_len);
		if (!eh->var_name)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		h->i += eh->var_name_len - 1;
		eh->var_value = ft_getenv(eh->var_name, state->envp);
	}
	if (eh->var_value)
	{
		eh->val_len = ft_strlen(eh->var_value);
		ensure_mem_for_cmd(h, state, ft_strlen(h->curr_cmd) + eh->val_len + 1);
		ft_strncat(h->curr_cmd, eh->var_value, eh->val_len);
		h->j = ft_strlen(h->curr_cmd);
	}
}

static void	separate_redir(t_parsehelper *h, t_shellstate *state)
{
	const char	*c = &state->input_string[h->i];
	const bool	space_before = (h->i > 0) && (*(c - 1) != ' ' && *(c
				- 1) != '\t');
	const bool	space_after = (*(c + 1) != ' ' && *(c + 1) != '\t' && *(c
				+ 1) != '\0');
	const bool	is_double_redir = (*(c + 1) == *c);

	ensure_mem_for_cmd(h, state, 5);
	if (space_before)
		h->curr_cmd[h->j++] = ' ';
	h->curr_cmd[h->j++] = *c;
	if (is_double_redir)
	{
		h->curr_cmd[h->j++] = *c;
		h->i++;
	}
	if (space_after)
		h->curr_cmd[h->j++] = ' ';
}

/**
 * @brief Checks if the current character is operator & puts the new command
 * in the commands array from the input string.
 */
static void	check_for_new_cmd(t_parsehelper *h, t_shellstate *state,
		t_envhelper *eh)
{
	if (eh->flags & (1 << REDIR_BIT))
		separate_redir(h, state);
	else if ((eh->flags & (1 << PIPE_BIT)) || (eh->flags & (1 << AND_BIT))
		|| (eh->flags & (1 << OR_BIT)))
	{
		h->curr_cmd[h->j] = '\0';
		h->commands[h->command_index++] = ft_strdup(h->curr_cmd);
		h->j = 0;
		if ((eh->flags & (1 << AND_BIT)) || (eh->flags & (1 << OR_BIT)))
			h->i++;
	}
	else if (eh->flags & (1 << ENVVAR_BIT))
	{
		handle_env_variable(h, state, eh);
		free(eh->var_name);
		if (eh->free_var_value)
			free(eh->var_value);
	}
	else
	{
		ensure_mem_for_cmd(h, state, 1);
		h->curr_cmd[h->j++] = state->input_string[h->i];
	}
}

/**
 * @brief Parses the characer from input string.
 * & Handles single and double quotes,
	escape sequences and environment variables.
 *
 */
void	parse_cmd_char(t_parsehelper *h, t_shellstate *state)
{
	t_envhelper	eh;

	init_char_flags(&eh, &state->input_string[h->i], h);
	if (eh.flags & (1 << TILDA_BIT))
		handle_tilda(h, state);
	else if ((eh.flags & (1 << ENVVAR_BIT)) && ft_checkdollar(state, h))
		eh.flags &= ~(1 << ENVVAR_BIT);
	else if (!(eh.flags & (1 << QUOTE_BIT)) && (h->in_single_quote
			|| h->in_double_quote))
	{
		if (eh.flags & (1 << ESCAPED_BIT))
			handle_escape_sequence(h, state->input_string);
		else if (h->in_double_quote && (eh.flags & (1 << ENVVAR_BIT)))
		{
			handle_env_variable(h, state, &eh);
			free(eh.var_name);
			if (eh.free_var_value)
				free(eh.var_value);
		}
		else
			h->curr_cmd[h->j++] = state->input_string[h->i];
	}
	else
		check_for_new_cmd(h, state, &eh);
	h->curr_cmd[h->j] = '\0';
}
