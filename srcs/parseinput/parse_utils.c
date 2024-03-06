/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 19:12:29 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 19:12:30 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "miniutils.h"

/**
 * @brief Ensures that there is enough memory for the command.
 * If not, reallocates double the size of the current memory.
 */
void	ensure_mem_for_cmd(t_parsehelper *h, t_shellstate *state,
		size_t additional_length)
{
	size_t	required_size;
	size_t	new_size;
	char	*new_command;

	required_size = h->j + additional_length + 1;
	if (required_size > h->alloc_size)
	{
		new_size = required_size * 2;
		new_command = ft_realloc(h->curr_cmd, h->alloc_size, new_size);
		if (!new_command)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		h->curr_cmd = new_command;
		h->alloc_size = new_size;
	}
}

/**
 * @brief Initializes the flags for the current character.
 * The flags are used to determine the state of the current character.
 */
void	init_char_flags(t_charflags *flags, char *c, t_parsehelper *h)
{
	ft_memset(flags, 0, sizeof(t_charflags));
	if ((*c == '\'' && !h->in_double_quote) || (*c == '"'
			&& !h->in_single_quote))
	{
		h->in_single_quote ^= (*c == '\'');
		h->in_double_quote ^= (*c == '"');
		flags->is_quote = true;
	}
	flags->is_escaped = (*c == '\\');
	flags->is_env_var = (*c == '$');
	if (!h->in_single_quote && !h->in_double_quote)
	{
		flags->is_pipe = (*c == '|' && *(c + 1) != '|');
		flags->is_and = (*c == '&' && *(c + 1) == '&');
		flags->is_or = (*c == '|' && *(c + 1) == '|');
	}
}

/**
 * @brief Matches the pattern with the string.
 */
bool	wildcard_match(const char *pattern, const char *str)
{
	if (ft_strcmp(pattern, "*") == 0 && *str == '.')
		return (false);
	while (*str)
	{
		if (*pattern == '*')
		{
			if (!*++pattern)
				return (true);
			while (*str && *pattern != *str)
				str++;
		}
		else
		{
			if (*pattern != *str)
				return (false);
			str++;
			pattern++;
		}
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*str);
}

int	ft_checkdollar(t_shellstate *s, t_parsehelper *h)
{
	if (!h->in_single_quote && !h->in_double_quote && (s->input_string[h->i
			+ 1] == ' ' || s->input_string[h->i + 1] == '\0'))
	{
		ensure_mem_for_cmd(h, s, 1);
		h->curr_cmd[h->j++] = s->input_string[h->i];
		return (1);
	}
	else if ((h->in_single_quote || h->in_double_quote) && (s->input_string[h->i
			+ 1] == '\'' || s->input_string[h->i + 1] == '"'
			|| s->input_string[h->i + 1] == ' ' || s->input_string[h->i
			+ 1] == '$'))
	{
		ensure_mem_for_cmd(h, s, 1);
		h->curr_cmd[h->j++] = s->input_string[h->i];
		return (1);
	}
	return (0);
}
