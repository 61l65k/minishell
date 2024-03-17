/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parentheses_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 04:26:31 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/17 04:26:32 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted(const char *str)
{
	return ((str[0] == '"' || str[0] == '\'') && str[strlen(str)
			- 1] == str[0]);
}

char	*trim_spaces(char *str)
{
	char	*end;

	while (isspace((unsigned char)*str))
		str++;
	if (*str == 0)
		return (str);
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;
	end[1] = '\0';
	return (str);
}

void	remove_empty_and_trim_strings(char **commands)
{
	int write_index = 0;
	for (int read_index = 0; commands[read_index] != NULL; read_index++)
	{
		char *cmd = commands[read_index];

		if (!is_quoted(cmd))
		{
			cmd = trim_spaces(cmd);
		}

		if (*cmd != '\0')
		{
			commands[write_index++] = cmd;
		}
		else
		{
			free(commands[read_index]);
		}
	}
	commands[write_index] = NULL;
}