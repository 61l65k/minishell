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

#include "minishell.h"

/**
 * @brief Sets up fd's for the child before executing the command.
 */
static void	handle_child_process(t_shellstate *s, t_exechelper *eh)
{
	s->is_child_process = true;
	if (check_pipedoc(s, eh))
	{
		if (handle_redirect(eh) == FAILURE)
			exit(EXIT_FAILURE);
	}
	dup_forward_fd(s, eh);
	if (!eh->pipe_doc && handle_redirect(eh) == FAILURE)
		exit(EXIT_FAILURE);
	eh->cmd_argv = lst_to_argv(s->parsed_cmds[eh->i]);
	if (!eh->cmd_argv)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	builtin_child(s, eh);
	ft_execvp(eh->cmd_argv[0], eh->cmd_argv, s->envp);
}

/**
 * @brief Handles fd's for the parent process after a fork.
 */
static void	handle_parent_process(t_shellstate *s, t_exechelper *eh)
{
	const t_list	*next_lst = s->parsed_cmds[eh->i + 1];

	signal(SIGINT, SIG_IGN);
	if (eh->fd_in != 0)
	{
		close(eh->fd_in);
		eh->fd_in = 0;
	}
	if (next_lst && next_lst->op_type == OP_PIPE)
	{
		eh->fd_in = eh->pipefd[0];
		close(eh->pipefd[1]);
	}
}

/**
 * @brief Forks and waits for child, if appropriate.
 * Waits for the child if we need to know its exit status before continuing.
 * Otherwise saves the pid to pid vector.
 */
static void	handle_fork(t_shellstate *s, t_exechelper *eh)
{
	pid_t			pid;
	const t_list	*next_lst = s->parsed_cmds[eh->i + 1];

	if (next_lst && next_lst->op_type == OP_PIPE)
	{
		if (pipe(eh->pipefd) < 0)
			ft_free_exit(s, ERR_PIPE, EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
		ft_free_exit(s, ERR_FORK, EXIT_FAILURE);
	if (pid == 0)
		handle_child_process(s, eh);
	else
	{
		handle_parent_process(s, eh);
		if (next_lst && (next_lst->op_type == OP_OR
				|| next_lst->op_type == OP_AND || check_pipedoc(s, eh)))
			wait_child(s, pid);
		else
		{
			vec_insert(&s->pid, &pid, 0);
		}
	}
}

void	handle_subshell(t_shellstate *s, t_exechelper *eh)
{
	int		ret;
	pid_t	pid;

	ret = 0;
	pid = fork();
	if (pid < 0)
		ft_free_exit(s, ERR_FORK, EXIT_FAILURE);
	if (pid == 0)
	{
		// printf("IN CHILD\n");
		s->in_subshell = true;
		s->parsed_cmds[eh->i]->sub_type = SUB_NONE;
		ret = ft_executecmd(s);
		exit(ret);
	}
	else
	{
		waitpid(pid, &ret, 0);
		eh->subshell_exit_status = WEXITSTATUS(ret);
		s->parsed_cmds[eh->i]->sub_type = SUB_NONE;
	}
}

/**
 * @brief Executes the parsed commands and handles all operators.
 * & Returns the exit status of the last command.
 */

int	ft_executecmd(t_shellstate *state)
{
	t_exechelper	eh;
	int				offset;

	// t_list			*tmp;
	/*for (t_list **lst = state->parsed_cmds; *lst; lst++)
	{
		tmp = *lst;
		while (tmp)
		{
			printf("cont:%s subtype:%d optype:%d iotype %d\n",
				tmp->content,tmp->sub_type, tmp->op_type, tmp->io_type);
			tmp = tmp->next;
		}
	}*/
	eh = (t_exechelper){0};
	// printf("CURRENT i:%d\n", eh.i);
	state->last_exit_status = 0;
	while (state->parsed_cmds[eh.i] != NULL)
	{
		eh.curr_cmd = state->parsed_cmds[eh.i];
		if (g_signal_flag || state->last_exit_status == SIGINT_EXIT)
			break ;
		if (eh.curr_cmd && eh.curr_cmd->sub_type == SUB_START)
		{
			offset = calculate_subshell_offset(&eh, state);
			handle_subshell(state, &eh);
			eh.i += offset;
			state->last_exit_status = eh.subshell_exit_status;
		}
		else
		{
			if (is_pipeline(state, eh.i) == true)
				handle_fork(state, &eh);
			else if (builtin_main(state,
					state->parsed_cmds[eh.i]) == BI_NOT_BUILTIN)
				handle_fork(state, &eh);
			check_operators(&eh, state);
			eh.pipe_doc = 0;
			eh.i++;
		}
	}
	wait_remaining_children(state);
	return (state->last_exit_status);
}
