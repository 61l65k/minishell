/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_heredoc.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:00:32 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/13 15:15:24 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_type.h"
#include "minishell.h"
#include "miniutils.h"
#include <signal.h>
#include <stdlib.h>

static void	open_heredoc(const char *delimiter, int fd_out)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			ft_fprintf(STDERR_FILENO, HDOC_DELIMMSG, delimiter);
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
}

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		g_signal_flag = SIGINT_EXIT;
		exit(SIGINT_EXIT);
	}
}

int	fork_heredoc(t_io *io)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s\n", strerror(errno));
		return (FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGINT, heredoc_signal_handler);
		open_heredoc(io->filename, io->fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		init_signals();
	}
	return (WEXITSTATUS(status));
}
