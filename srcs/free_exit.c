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
	int	i;

	i = -1;
	if (state->parsed_cmds)
	{
		while (state->parsed_cmds[++i])
			ft_lstclear(state->parsed_cmds, free);
		free(state->parsed_cmds);
	}
	state->parsed_cmds = NULL;
	free_str_array(state->parsed_args);
	state->parsed_args = NULL;
	state->cmd_count = 0;
	if (state->operators)
		free(state->operators);
	state->operators = NULL;
	state->operator_count = 0;
}

char	**list_to_array(t_list *list)
{
	int		size;
	char	**array;
	int		i;

	size = ft_lstsize(list);
	array = malloc((size + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (list)
	{
		array[i++] = list->content;
		list = list->next;
	}
	array[i] = NULL;
	return (array);
}
