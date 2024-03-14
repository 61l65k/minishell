/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_apply.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 23:26:52 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/13 15:15:50 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include "io_type.h"
#include "builtin.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include "vec/vec.h"

int	back_up_main_process_fds(t_command *command)
{
	command->fd_stdin_backup = dup(STDIN_FILENO);
	if (command->fd_stdin_backup == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s\n", strerror(errno));
		return (-1);
	}
	command->fd_stdout_backup = dup(STDOUT_FILENO);
	if (command->fd_stdout_backup == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

int	fork_heredoc(t_io *io);

int	apply_main_process_redirections(t_command *command)
{
	t_io	*io_current;
	size_t	i;
	int		heredoc_status;

	if (back_up_main_process_fds(command) == -1)
		return (FAILURE);
	i = 0;
	while (i < command->io_vec.len)
	{
		io_current = (t_io *)vec_get(&command->io_vec, i);
		io_current->fd = -1;
		if (open_file(io_current) == -1)
			return (FAILURE);
		if (dup_fd(io_current) == -1)
			return (FAILURE);
		if (io_current->type == IO_HEREDOC)
		{
			heredoc_status = fork_heredoc(io_current);
			if (heredoc_status != 0)
				return (heredoc_status);
		}
		i++;
	}
	return (0);
}

int	dup_fd(t_io *io)
{
	int	fd_to_dup;

	if (io->type == IO_OUT_TRUNC || io->type == IO_OUT_APPEND)
		fd_to_dup = STDOUT_FILENO;
	else if (io->type == IO_INPUT)
		fd_to_dup = STDIN_FILENO;
	else
		return (0);
	if (dup2(io->fd, fd_to_dup) == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

int	open_file(t_io *io)
{
	if (io->ambiguous_redirect == true)
	{
		ft_fprintf(STDERR_FILENO, ERR_AMBIGUOUS_REDIRECT, io->filename);
		return (-1);
	}
	if (io->type == IO_OUT_TRUNC)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (io->type == IO_OUT_APPEND)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (io->type == IO_INPUT)
		io->fd = open(io->filename, O_RDONLY);
	else
		return (0);
	if (io->fd == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: %s\n", io->filename, strerror(errno));
		return (-1);
	}
	return (0);
}
