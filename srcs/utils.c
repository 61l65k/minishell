/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:48:56 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/21 23:48:58 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees all & Exit the shell with an error message or with success.
 */
void	ft_free_exit(t_ShellState *state, const char *errorMsg, int exitCode)
{
	if (errorMsg || exitCode != 0)
		perror(errorMsg);
	ft_free_resets(state);
	exit(exitCode);
}

/**
 * @brief Frees the shellState and resets the terminal.
 */
void	ft_free_resets(t_ShellState *state)
{
	int	i;

	i = 0;
	if (state->parsed_args)
	{
		while (state->parsed_args[i])
			free(state->parsed_args[i++]);
		free(state->parsed_args);
	}
}

/**
 * @brief Trims the leading and trailing spaces from a string.
 * & returns a new string.
 */
char	*trim_spaces(char *str)
{
	char	*start;
	char	*end;
	size_t	length;
	char	*trimmed;

	if (str == NULL)
		return (NULL);
	start = str;
	while (*start && *start == ' ')
		start++;
	if (*start == '\0')
		return (ft_strdup(""));
	end = start + ft_strlen(start) - 1;
	while (end > start && *end == ' ')
		end--;
	length = end - start + 1;
	trimmed = malloc(length + 1);
	if (trimmed == NULL)
		return (NULL);
	ft_strlcpy(trimmed, start, length + 1);
	return (trimmed);
}
