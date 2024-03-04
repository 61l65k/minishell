/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/04 18:39:53 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Handles execution for the child process. if pipe is used,
	it will be used.
 */
static void	handle_child_process(t_shellstate *state, t_exechelper *helper)
{
	char	**cmd_array;

	if (helper->fd_in != 0)
	{
		dup2(helper->fd_in, STDIN_FILENO);
		close(helper->fd_in);
	}
	if (helper->i < state->cmd_count - 1
		&& state->operators[helper->i] == OP_PIPE)
	{
		close(helper->pipefd[0]);
		dup2(helper->pipefd[1], STDOUT_FILENO);
		close(helper->pipefd[1]);
	}
	else
	{
		if (helper->i < state->cmd_count - 1)
		{
			close(helper->pipefd[0]);
			close(helper->pipefd[1]);
		}
	}
	cmd_array = list_to_array(helper->cmd_args);
	if (!cmd_array)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	// ft_builtin_cmdhandler(state, helper, true);
	execute_cmd(cmd_array[0], cmd_array, state->envp);
	free_str_array(cmd_array);
}

/**

	* @brief Handles execution for the parent process,
	if piped, closes the pipe and frees the memory.
 */
static void	handle_parent_process(t_shellstate *state, t_exechelper *helper)
{
	if (helper->fd_in != 0)
		close(helper->fd_in);
	if (helper->i < state->cmd_count - 1)
	{
		helper->fd_in = helper->pipefd[0];
		close(helper->pipefd[1]);
	}
	// ft_lstclear(&helper->cmd_args, free);
}

/**
 * @brief Handles forking & executing the child & parent processes.
 */
static void	handle_fork(t_shellstate *s, t_exechelper *h)
{
	if (h->i < s->cmd_count - 1)
	{
		if (pipe(h->pipefd) < 0)
			ft_free_exit(s, ERR_PIPE, EXIT_FAILURE);
	}
	h->pid_current = fork();
	if (h->pid_current < 0)
		ft_free_exit(s, ERR_FORK, EXIT_FAILURE);
	if (h->pid_current == 0)
		handle_child_process(s, h);
	else
	{
		handle_parent_process(s, h);
		if (s->operators[h->i] == OP_OR || s->operators[h->i] == OP_AND)
			wait_child(s, h->pid_current);
		else
			vec_insert(&s->pid, &h->pid_current, 0);
	}
}

/**
 * @brief Checks the operators and skips the commands if needed.
 */
static void	check_operators(t_exechelper *h, t_shellstate *s)
{
	if (h->i < s->operator_count)
	{
		if (s->operators[h->i] == OP_AND && s->last_exit_status != 0)
		{
			while (h->i < s->operator_count && s->operators[h->i] != OP_OR)
				h->i++;
		}
		else if (s->operators[h->i] == OP_OR && s->last_exit_status == 0)
		{
			while (h->i < s->operator_count && s->operators[h->i] != OP_AND)
				h->i++;
		}
	}
}

/**
 * @brief Executes the parsed commands and handles all operators.
 * & Returns the exit status of the last command.
 */
int	ft_executecmd(t_shellstate *state)
{
	t_exechelper	h;

	ft_memset(&h, 0, sizeof(t_exechelper));
	while (h.i < state->cmd_count)
	{
		h.cmd_args = state->parsed_cmds[h.i];
		if (!h.cmd_args)
			ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
		// if (ft_builtin_cmdhandler(state, &h, false))
		//		free_and_null_str_array(&h.cmd_args);
		else
			handle_fork(state, &h);
		check_operators(&h, state);
		h.i++;
	}
	wait_remaining_children(state);
	return (state->last_exit_status);
}
