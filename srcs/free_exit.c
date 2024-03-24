/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:48:56 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/15 22:25:12 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees all & Exit the shell. Prints msg with perror if exit_code != 0.
 */
void	ft_free_exit(t_shellstate *state, const char *msg, int exit_code)
{
	if (msg && exit_code != EXIT_SUCCESS)
		perror(msg);
	else if (msg)
		ft_fprintf(STDERR_FILENO, "%s\n", msg);
	if (state->pid.len > 0)
		wait_remaining_children(state);
	ft_free_resets(state);
	free_str_array(state->envp);
	state->envp = NULL;
	vec_free(&state->pid);
	exit(exit_code);
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
 * @brief Frees/nulls/resets all members of the shellstate struct
 * that do not need to be preserved and/or should be reset after each prompt.
 */
void	ft_free_resets(t_shellstate *state)
{
	free(state->input_string);
	state->input_string = NULL;
	if (state->parsed_cmds)
		free_parsed_cmds_list(state->parsed_cmds);
	state->parsed_cmds = NULL;
	free_str_array(state->parsed_args);
	state->parsed_args = NULL;
	state->cmd_count = 0;
	if (state->operators)
		free(state->operators);
	state->operators = NULL;
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

/**
 * @brief Returns redirect-free argv from arg_list that can be passed to execve
 * NOTE: Caller should only free the array, not the strings, as they are
 * not copied but point to the same memory as the list.
 */
char	**lst_to_argv(t_list *arg_list)
{
	char	**argv;

	argv = lst_to_2darray(arg_list);
	if (!argv)
		return (NULL);
	remove_redirections_from_args(argv, arg_list);
	return (argv);
}
