/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/08 19:21:37 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "io_type.h"
#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * @brief Duplicates the fd's forward if we are in a pipeline.
 * so next command can read from it.
 * & closes the pipefd if we are not in a pipeline.
 */
static void	dup_forward_fd(t_shellstate *s, t_exechelper *h)
{
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
	else if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		close(h->pipefd[0]);
		close(h->pipefd[1]);
	}
}

/**
 * @brief Sets up fd's for the child before executing the command.
 */
static void	handle_child_process(t_shellstate *s, t_exechelper *h)
{
	s->is_child_process = true;
	if (check_pipedoc(s, h))
	{
		if (handle_redirect(h) == FAILURE)
			exit(EXIT_FAILURE);
	}
	dup_forward_fd(s, h);
	if (!h->pipe_doc && handle_redirect(h) == FAILURE)
		exit(EXIT_FAILURE);
	h->cmd_arr = lst_to_argv(s->parsed_cmds[h->i]);
	if (!h->cmd_arr)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	builtin_child(s, h);
	ft_execvp(h->cmd_arr[0], h->cmd_arr, s->envp);
}

/**
 * @brief Handles fd's for the parent process after a fork.
 */
static void	handle_parent_process(t_shellstate *s, t_exechelper *h)
{
	signal(SIGINT, SIG_IGN);
	if (h->fd_in != 0)
	{
		close(h->fd_in);
		h->fd_in = 0;
	}
	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		h->fd_in = h->pipefd[0];
		close(h->pipefd[1]);
	}
}

/**
 * @brief Forks and waits for child, if appropriate.
 * Waits for the child if we need to know its exit status before continuing.
 * Otherwise saves the pid to pid vector.
 */
static void	handle_fork(t_shellstate *s, t_exechelper *h)
{
	pid_t	pid;

	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		if (pipe(h->pipefd) < 0)
			ft_free_exit(s, ERR_PIPE, EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
		ft_free_exit(s, ERR_FORK, EXIT_FAILURE);
	if (pid == 0)
		handle_child_process(s, h);
	else
	{
		handle_parent_process(s, h);
		if (s->operators[h->i] == OP_OR || s->operators[h->i] == OP_AND
			|| check_pipedoc(s, h))
			wait_child(s, pid);
		else
			vec_insert(&s->pid, &pid, 0);
	}
}

/**
 * @brief Executes the parsed commands and handles all operators.
 * & Returns the exit status of the last command.
 */
int	ft_executecmd(t_shellstate *state)
{
	t_exechelper	eh;

	eh = (t_exechelper){0};
	state->last_exit_status = 0;
	while (eh.i < state->cmd_count)
	{
		eh.curr_cmd = state->parsed_cmds[eh.i];
		if (g_signal_flag || state->last_exit_status)
			break ;
		if (is_pipeline(state, &eh) == true)
			handle_fork(state, &eh);
		else if (builtin_main(state,
				state->parsed_cmds[eh.i]) == BI_NOT_BUILTIN)
			handle_fork(state, &eh);
		check_operators(&eh, state);
		eh.pipe_doc = 0;
		eh.i++;
	}
	wait_remaining_children(state);
	return (state->last_exit_status);
}
