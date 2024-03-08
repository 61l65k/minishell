/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/07 22:34:07 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Sets up fd's for the child before executing the command.
 */
static void	handle_child_process(t_shellstate *s, t_exechelper *h)
{
	s->is_child_process = true;
	h->cmd_arr = lst_to_2darray(s->parsed_cmds[h->i]);
	if (!h->cmd_arr)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	if (apply_cmd_redirections(h, s, s->parsed_cmds[h->i]) == FAILURE)
		exit(EXIT_FAILURE);
	if (h->fd_in != 0)
	{
		dup2(h->fd_in, STDIN_FILENO);
		close(h->fd_in);
	}
	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		close(h->pipefd[0]);
		dup2(h->pipefd[1], STDOUT_FILENO);
		close(h->pipefd[1]);
	}
	else if (h->i < s->cmd_count - 1)
	{
		close(h->pipefd[0]);
		close(h->pipefd[1]);
	}
	builtin_child(s, h);
	ft_execvp(h->cmd_arr[0], h->cmd_arr, s->envp);
}

/**
 * @brief Handles fd's for the parent process after a fork.
 */
static void	handle_parent_process(t_shellstate *state, t_exechelper *helper)
{
	if (helper->fd_in != 0)
	{
		close(helper->fd_in);
		helper->fd_in = 0;
	}
	if (helper->i < state->cmd_count - 1 && state->operators[helper->i] == OP_PIPE)
	{
		helper->fd_in = helper->pipefd[0];
		close(helper->pipefd[1]);
	}
}

/**
 * @brief Forks and waits for child, if appropriate.
 * Waits for the child if we need to know its exit status before continuing.
 * Otherwise saves the pid to pid vector.
 */
static void	handle_fork(t_shellstate *s, t_exechelper *h)
{
	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
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

		if (is_pipeline(state, &h) == true)
			handle_fork(state, &h);
		else if (builtin_main(state, state->parsed_cmds[h.i]) == BI_NOT_BUILTIN)
			handle_fork(state, &h);
		check_operators(&h, state);
		h.i++;
	}
	wait_remaining_children(state);
	return (state->last_exit_status);
}
