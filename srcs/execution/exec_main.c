/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/29 20:00:30 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Handles the built-in commands.
 * & Returns 1 if the command is handled, otherwise 0.
 */
int	ft_cmdhandler(t_shellstate *state, char **parsed)
{
	if (!parsed || !parsed[0])
		return (FOUNDCMD);
	if (ft_strcmp(parsed[0], "exit") == 0)
		ft_free_exit(state, NULL, EXIT_SUCCESS);
	else if (ft_strcmp(parsed[0], "cd") == 0)
		return (change_dir(parsed[1], state->envp), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "help") == 0)
		return (printf(HELP_MSG), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "hello") == 0)
		return (printf(HELLO_MSG, ft_getenv("USER", state->envp)), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "unset") == 0)
		return (builtin_unset(parsed, state), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "export") == 0)
		return (builtin_export(parsed, state), FOUNDCMD);
	return (SUCCESS);
}

/**
 * @brief Handles execution for the child process. if pipe is used,
	it will be used.
 */
void	handle_child_process(t_shellstate *state, t_exechelper *helper)
{
	if (helper->fd_in != 0)
	{
		dup2(helper->fd_in, STDIN_FILENO);
		close(helper->fd_in);
	}
	if (helper->i < state->cmd_count - 1
		&& ft_strcmp(state->operators[helper->i], "|") == 0)
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
	execute_cmd(helper->cmd_args[0], helper->cmd_args, state->envp);
}

/**

	* @brief Handles execution for the parent process,
	if piped, closes the pipe and frees the memory.
 */
void	handle_parent_process(t_shellstate *state, t_exechelper *helper)
{
	int	j;

	j = 0;
	if (helper->fd_in != 0)
		close(helper->fd_in);
	if (helper->i < state->cmd_count - 1)
	{
		helper->fd_in = helper->pipefd[0];
		close(helper->pipefd[1]);
	}
	while (helper->cmd_args[j])
		free(helper->cmd_args[j++]);
	free(helper->cmd_args);
}

/**
 * @brief Handles foking & executing the child & parent processes.
 * & Returns the exit status of the last command.
 */
static int	handle_fork(t_shellstate *state, t_exechelper *h)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (h->i < state->cmd_count - 1)
	{
		if (pipe(h->pipefd) < 0)
			ft_free_exit(state, ERR_PIPE, EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
		ft_free_exit(state, ERR_FORK, EXIT_FAILURE);
	if (pid == 0)
		handle_child_process(state, h);
	else
	{
		vec_push(&state->pid, &pid);
		handle_parent_process(state, h);
		if (h->i == state->cmd_count - 1 || ft_strcmp(state->operators[h->i],
				"|") != 0)
			return (waitpid(pid, &status, 0), WEXITSTATUS(status));
	}
	return (0);
}

/**
 * @brief Executes the parsed commands and handles all operators.
 * & Returns the exit status of the last command.
 */
int	ft_executecmd(t_shellstate *state)
{
	t_exechelper	h;
	pid_t			pid;

	vec_new(&state->pid, state->cmd_count, sizeof(pid_t));
	ft_memset(&h, 0, sizeof(t_exechelper));
	while (h.i < state->cmd_count)
	{
		h.cmd_args = ft_split(state->parsed_args[h.i], ' ');
		if (!h.cmd_args)
			ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
		if (ft_cmdhandler(state, h.cmd_args) == FOUNDCMD)
			free_str_array(h.cmd_args);
		else
			h.status = handle_fork(state, &h);
		h.i++;
		if (h.i < state->operator_count && ((ft_strcmp(state->operators[h.i],
						"&&") == 0 && h.status != 0)
				|| (ft_strcmp(state->operators[h.i], "||") == 0
					&& h.status == 0)))
			break ;
	}
	while (state->pid.len > 0)
		vec_pop(&pid, &state->pid);
	return (vec_free(&state->pid), h.status);
}
