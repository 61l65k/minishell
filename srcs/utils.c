/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:48:56 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/01 13:08:09 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	i;

	i = 0;
	if (state->parsed_args)
	{
		while (state->parsed_args[i])
			free(state->parsed_args[i++]);
		free(state->parsed_args);
	}
	if (state->operators)
	{
		i = 0;
		while (state->operators[i])
			free(state->operators[i++]);
		free(state->operators);
	}
}

/**
 * @brief Trims the leading and trailing spaces from a string.
 * & returns a new string.
 */
char	*trim_spaces(const char *str)
{
	const char	*start = str;
	const char	*end = str + ft_strlen(str) - 1;
	size_t		length;

	if (str == NULL)
		return (NULL);
	while (*start && *start == ' ')
		start++;
	if (*start == '\0')
		return (ft_strdup(""));
	while (end > start && *end == ' ')
		end--;
	length = end - start + 1;
	return (ft_strndup(start, length));
}
