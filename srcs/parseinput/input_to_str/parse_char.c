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

#include "minishell.h"
#include "miniutils.h"

static void	handle_escape_sequence(t_parsehelper *ph, const char *input_string)
{
	ph->i++;
	if (input_string[ph->i] == 'n')
		ph->curr_cmd[ph->j++] = '\n';
	else if (input_string[ph->i] == 't')
		ph->curr_cmd[ph->j++] = '\t';
	else if (input_string[ph->i] == 'r')
		ph->curr_cmd[ph->j++] = '\r';
	else if (input_string[ph->i] == '\\')
		ph->curr_cmd[ph->j++] = '\\';
	else if (input_string[ph->i] == '"')
		ph->curr_cmd[ph->j++] = '\"';
	else if (input_string[ph->i] == '\'')
		ph->curr_cmd[ph->j++] = '\'';
	else
		ph->curr_cmd[ph->j++] = input_string[ph->i];
}

void	get_env_var_value(t_shellstate *state, t_envhelper *eh,
		t_parsehelper *ph)
{
	if (state->input_string[ph->i] == '?')
	{
		eh->var_value = ft_itoa(state->last_exit_status);
		if (!eh->var_value)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		eh->free_var_value = true;
	}
	else
	{
		eh->var_name_len = ft_envlen(state->input_string + ph->i);
		eh->var_name = ft_strndup(state->input_string + ph->i,
				eh->var_name_len);
		if (!eh->var_name)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		ph->i += eh->var_name_len - 1;
		eh->var_value = ft_getenv(eh->var_name, state->envp);
	}
}

static void	expand_env_variable(t_parsehelper *ph, t_shellstate *s,
		t_envhelper *eh)
{
	ph->i += 1;
	get_env_var_value(s, eh, ph);
	if (eh->var_value)
	{
		eh->val_len = ft_strlen(eh->var_value);
		ensure_mem_for_buff(ph, s, ft_strlen(ph->curr_cmd) + eh->val_len + 1,
			ph->curr_cmd);
		ft_strncat(ph->curr_cmd, eh->var_value, eh->val_len);
		ph->j = ft_strlen(ph->curr_cmd);
	}
	free(eh->var_name);
	if (eh->free_var_value)
		free(eh->var_value);
}

static void	separate_redir_with_spaces(t_parsehelper *ph, t_shellstate *state)
{
	const char	*c = &state->input_string[ph->i];
	const bool	space_before = (ph->i > 0) && (*(c - 1) != ' ' && *(c
					- 1) != '\t');
	const bool	space_after = (*(c + 1) != ' ' && *(c + 1) != '\t' && *(c
					+ 1) != '\0');
	const bool	is_double_redir = (*(c + 1) == *c);

	ensure_mem_for_buff(ph, state, 5, ph->curr_cmd);
	if (space_before)
		ph->curr_cmd[ph->j++] = ' ';
	ph->curr_cmd[ph->j++] = *c;
	if (is_double_redir)
	{
		ph->curr_cmd[ph->j++] = *c;
		ph->i++;
	}
	if (space_after)
		ph->curr_cmd[ph->j++] = ' ';
}

static void	handle_non_quoted_char(t_parsehelper *ph, t_shellstate *state,
		int flags, t_envhelper *eh)
{
	if (flags & (1 << REDIR_BIT))
		separate_redir_with_spaces(ph, state);
	else if ((flags & (1 << PIPE_BIT)) || (flags & (1 << AND_BIT))
		|| (flags & (1 << OR_BIT)))
	{
		ph->curr_cmd[ph->j] = '\0';
		ph->commands[ph->command_index++] = ft_strdup(ph->curr_cmd);
		ph->j = 0;
		if ((flags & (1 << AND_BIT)) || (flags & (1 << OR_BIT)))
			ph->i++;
	}
	else if (flags & (1 << ENVVAR_BIT))
	{
		expand_env_variable(ph, state, eh);
	}
	else
	{
		ensure_mem_for_buff(ph, state, 1, ph->curr_cmd);
		ph->curr_cmd[ph->j++] = state->input_string[ph->i];
	}
}

/**
 * @brief Parses the characer from input string.
 * & Handles single and double quotes,
	escape sequences and environment variables. And turns the the
 *
 */
void	parse_character(t_parsehelper *ph, t_shellstate *s)
{
	t_envhelper	eh;
	int			flags;

	eh = (t_envhelper){0};
	init_char_flags(&flags, &s->input_string[ph->i], ph);
	if (flags & (1 << TILDA_BIT))
		handle_tilda(ph, s);
	else if ((flags & (1 << ENVVAR_BIT)) && ft_checkdollar(s, ph))
		flags &= ~(1 << ENVVAR_BIT);
	else if (!(flags & (1 << QUOTE_BIT)) && (ph->in_single_quote
			|| ph->in_double_quote))
	{
		if (flags & (1 << ESCAPED_BIT))
			handle_escape_sequence(ph, s->input_string);
		else if (ph->in_double_quote && (flags & (1 << ENVVAR_BIT)))
		{
			expand_env_variable(ph, s, &eh);
		}
		else
			ph->curr_cmd[ph->j++] = s->input_string[ph->i];
	}
	else
		handle_non_quoted_char(ph, s, flags, &eh);
	ph->curr_cmd[ph->j] = '\0';
}
