/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_apply.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 23:26:52 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/08 17:55:11 by ttakala          ###   ########.fr       */
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
#include "vec/vec.h"

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
		io_current->fd = -1;
		open_file(io_current);
		dup_fd(io_current);
		if (io_current->type == IO_IN_HEREDOC)
			open_heredoc(io_current->filename, -1);
		i++;
	}
	return (0);
}

int	dup_fd(t_io *io)
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

int	open_file(t_io *io)
{
	if (io->type == IO_OUT_TRUNC)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (io->type == IO_OUT_APPEND)
		io->fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (io->type == IO_IN_TRUNC)
		io->fd = open(io->filename, O_RDONLY);
	else
		return (0);
	if (io->fd == -1)
	{
		ft_fprintf(2, "minishell: %s: %s: %s\n", io->filename, strerror(errno));
		return (-1);
	}
	return (0);
}

int	open_heredoc(const char *delimiter, int fd_out)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: warning: here-document ");
			ft_fprintf(STDERR_FILENO,
				"delimited by end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (fd_out != -1)
			ft_fprintf(fd_out, "%s\n", line);
		free(line);
	}
	return (0);
}
