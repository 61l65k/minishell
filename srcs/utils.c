/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:48:56 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/01 13:56:43 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "miniutils.h"

/**
 * @brief Frees all & Exit the shell with an error message or with success.
 */
void	ft_free_exit(t_shellstate *state, const char *errorMsg, int exitCode)
{
	if (errorMsg)
		perror(errorMsg);
	ft_free_resets(state);
	free_str_array(state->envp);
	state->envp = NULL;
	vec_free(&state->pid);
	exit(exitCode);
}

/**
 * @brief Frees the shellState and resets the terminal.
 */
void	ft_free_resets(t_shellstate *state)
{
	free_str_array(state->parsed_args);
	state->parsed_args = NULL;
	state->cmd_count = 0;
	if (state->operators)
		free(state->operators);
	state->operators = NULL;
	state->operator_count = 0;
}

static char	*allocate_trimmed_string(t_trimhelper *t)
{
	while (t->i < t->length)
	{
		if ((t->start[t->i] == '\'' && !t->in_double_quote)
			|| (t->start[t->i] == '"' && !t->in_single_quote))
		{
			t->in_single_quote ^= (t->start[t->i] == '\'');
			t->in_double_quote ^= (t->start[t->i] == '"');
			t->i++;
			continue ;
		}
		if (t->start[t->i] != ' ' || (t->in_single_quote || t->in_double_quote)
			|| !t->space_found)
		{
			t->trimmed[t->j++] = t->start[t->i];
			t->space_found = (t->start[t->i] == ' ' && !(t->in_single_quote
						|| t->in_double_quote));
		}
		t->i++;
	}
	t->trimmed[t->j] = '\0';
	return (t->trimmed);
}

char	*trim_command(const char *str)
{
	t_trimhelper	t;

	if (str == NULL)
		return (NULL);
	ft_memset(&t, 0, sizeof(t_trimhelper));
	t.start = str;
	t.end = str + ft_strlen(str) - 1;
	while (*t.start && (*t.start == ' ' || *t.start == '\t'))
		t.start++;
	while (t.end > t.start && (*t.end == ' ' || *t.end == '\t'))
		t.end--;
	t.length = t.end - t.start + 1;
	t.trimmed = malloc(t.length + 1);
	if (!t.trimmed)
		return (NULL);
	return (allocate_trimmed_string(&t));
}
