/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:48:56 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 11:54:02 by ttakala          ###   ########.fr       */
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

void	free_parsed_cmds_list(t_list **array_of_lists)
{
	int	i;

	i = 0;
	while (array_of_lists[i])
	{
		ft_lstclear(&array_of_lists[i], free);
		i++;
	}
	free(array_of_lists);
	array_of_lists = NULL;
}

/**
 * @brief Frees the shellState and resets the terminal.
 */
void	ft_free_resets(t_shellstate *state)
{
	if (state->parsed_cmds)
	{
		free_parsed_cmds_list(state->parsed_cmds);
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

/**
 * @brief Returns NULL-terminated array of pointers to the strings in list.
 * NOTE: Caller should only free the array, not the strings, as they are
 * not copied but point to the same memory as the list.
 */
char	**lst_to_2darray(t_list *list)
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
