/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_apply.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 23:26:52 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/08 11:53:05 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "io_type.h"
#include "builtin.h"
#include "vec/vec.h"

static int	dup_fd(t_io *io)
{
	int	fd_to_dup;

	fd_to_dup = -1;
	if (io->type == IO_OUT_TRUNC || io->type == IO_OUT_APPEND)
		fd_to_dup = STDOUT_FILENO;
	else if (io->type == IO_IN_TRUNC || io->type == IO_IN_HEREDOC)
		fd_to_dup = STDIN_FILENO;
	if (fd_to_dup == -1 || io->fd == -1)
		return (-1);
	if (dup2(io->fd, fd_to_dup) == -1)
	{
		ft_fprintf(2, "minishell: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

static int	open_file(t_io *io)
{
	io->fd = -1;
	if (io->type == IO_OUT_TRUNC)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (io->type == IO_OUT_APPEND)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (io->type == IO_IN_TRUNC)
		io->fd = open(io->filename, O_RDONLY);
	else if (io->type == IO_IN_HEREDOC)
		io->fd = (open(io->filename, O_RDONLY));
	if (io->fd == -1)
	{
		ft_fprintf(2, "minishell: %s: %s: %s\n", io->filename, strerror(errno));
		return (-1);
	}
	return (0);
}

int	apply_main_process_redirections(t_command *command)
{
	t_io	*io_current;
	size_t	i;

	command->fd_stdin_backup = dup(STDIN_FILENO);
	command->fd_stdout_backup = dup(STDOUT_FILENO);
	if (command->fd_stdin_backup == -1 || command->fd_stdout_backup == -1)
	{
		ft_fprintf(2, "minishell: %s\n", strerror(errno));
		return (-1);
	}
	i = 0;
	while (i < command->io_vec.len)
	{
		io_current = (t_io *)vec_get(&command->io_vec, i);
		open_file(io_current);
		dup_fd(io_current);
		i++;
	}
	return (0);
}
