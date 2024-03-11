/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 05:32:28 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/07 05:32:29 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_flag = 130;
		exit(SIGINT_EXIT);
	}
}

static void	heredoc_child_process(t_hdochelper *h)
{
	char	*line;

	signal(SIGINT, heredoc_signal_handler);
	close(h->pipe_fds[0]);
	while (true)
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			ft_fprintf(STDERR_FILENO, HDOC_DELIMMSG, h->delimiter);
			break ;
		}
		if (ft_strcmp(line, h->delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(h->pipe_fds[1], line, ft_strlen(line));
		write(h->pipe_fds[1], "\n", 1);
		free(line);
	}
	close(h->pipe_fds[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_higher_process(t_hdochelper *hd, t_exechelper *h)
{
	int	status;

	sigaction(SIGINT, &hd->s->ignoreaction, &hd->s->sigaction);
	close(hd->pipe_fds[1]);
	waitpid(hd->pid, &status, 0);
	sigaction(SIGINT, &hd->s->sigaction, NULL);
	if (WEXITSTATUS(status) == SIGINT_EXIT)
		return (close(hd->pipe_fds[0]), close(h->pipefd[0]),
			close(h->pipefd[1]), exit(SIGINT_EXIT));
	hd->rh->fd = hd->pipe_fds[0];
	update_fds(NULL, hd->rh, false);
}

void	handle_heredoc(t_redirecthelper *rh, char *delimiter, t_shellstate *s,
		t_exechelper *h)
{
	t_hdochelper	hd;

	hd = (t_hdochelper){0};
	hd.s = s;
	hd.rh = rh;
	hd.delimiter = delimiter;
	if (!delimiter)
		return (ft_putstr_fd(ERR_HEREDOC_DELIMITER, STDERR_FILENO));
	if (pipe(hd.pipe_fds) == -1)
		return (perror("pipe"), exit(EXIT_FAILURE));
	hd.pid = fork();
	if (hd.pid == -1)
		return (perror("fork() heredoc"), close(hd.pipe_fds[0]),
			close(hd.pipe_fds[1]), exit(EXIT_FAILURE));
	if (hd.pid == 0)
		heredoc_child_process(&hd);
	else
		handle_higher_process(&hd, h);
}
