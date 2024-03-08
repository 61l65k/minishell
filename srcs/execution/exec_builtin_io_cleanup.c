/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_cleanup.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 23:26:52 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/08 11:52:08 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "io_type.h"
#include "builtin.h"
#include "vec/vec.h"

int	restore_main_process_fds(t_command *command)
{
	t_io	*io_current;
	size_t	i;

	i = 0;
	while (i < command->io_vec.len)
	{
		io_current = (t_io *)vec_get(&command->io_vec, i);
		if (io_current->fd != -1)
			close(io_current->fd);
		i++;
	}
	if (command->fd_stdin_backup != -1)
	{
		dup2(command->fd_stdin_backup, STDIN_FILENO);
		close(command->fd_stdin_backup);
	}
	if (command->fd_stdout_backup != -1)
	{
		dup2(command->fd_stdout_backup, STDOUT_FILENO);
		close(command->fd_stdout_backup);
	}
	return (0);
}