/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/28 19:29:12 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

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
		return (change_dir(parsed[1]), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "help") == 0)
		return (printf(HELP_MSG), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "hello") == 0)
		return (printf(HELLO_MSG, getenv("USER")), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "unset") == 0)
		return (builtin_unset(parsed, state), FOUNDCMD);
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
	if (helper->i < state->cmd_count - 1)
	{
		close(helper->pipefd[0]);
		dup2(helper->pipefd[1], STDOUT_FILENO);
		close(helper->pipefd[1]);
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

static void	handle_fork(t_shellstate *state, t_exechelper *h)
{
	if (h->i < state->cmd_count - 1)
	{
		if (pipe(h->pipefd) < 0)
			ft_free_exit(state, ERR_PIPE, EXIT_FAILURE);
	}
	h->pid = fork();
	if (h->pid < 0)
		ft_free_exit(state, ERR_FORK, EXIT_FAILURE);
	if (h->pid == 0)
		handle_child_process(state, h);
	else
		handle_parent_process(state, h);
}

int	ft_executecmd(t_shellstate *state)
{
	t_exechelper	h;

	ft_memset(&h, 0, sizeof(t_exechelper));
	while (h.i < state->cmd_count)
	{
		h.cmd_args = ft_split(state->parsed_args[h.i], ' ');
		if (!h.cmd_args)
			ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
		if (ft_cmdhandler(state, h.cmd_args) == FOUNDCMD)
			free_str_array(h.cmd_args);
		else
			handle_fork(state, &h);
		h.i++;
	}
	waitpid(h.pid, &h.status, 0);
	return (WEXITSTATUS(h.status));
}
