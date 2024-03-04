/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:33:20 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/04 18:37:35 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Waits for any remaining child processes to complete.
 * & Updates state.last_exit_status.
 */
void	wait_remaining_children(t_shellstate *state)
{
	pid_t	pid_current;
	int		status;

	pid_current = 0;
	status = 0;
	while (state->pid.len > 0)
	{
		vec_pop(&pid_current, &state->pid);
		waitpid(pid_current, &status, 0);
		state->last_exit_status = WEXITSTATUS(status);
	}
}

/**
 * Waits for a single child process to complete.
 * & Updates state.last_exit_status.
 */
void	wait_child(t_shellstate *state, pid_t pid)
{
	int		status;

	status = 0;
	waitpid(pid, &status, 0);
	state->last_exit_status = WEXITSTATUS(status);
}
