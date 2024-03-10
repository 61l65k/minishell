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

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
		_exit(EXIT_SUCCESS);
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

static void	handle_main_process(t_hdochelper *h)
{
	sigaction(SIGINT, &h->s->ignoreaction, &h->s->sigaction);
	h->s->in_heredoc = true;
	close(h->pipe_fds[1]);
	waitpid(h->pid, NULL, 0);
	sigaction(SIGINT, &h->s->sigaction, NULL);
	h->s->in_heredoc = false;
	h->rh->fd = h->pipe_fds[0];
	redirect_fd(NULL, h->rh, false);
}

int	handle_heredoc(t_redirecthelper *rh, char *delimiter, t_shellstate *s)
{
	t_hdochelper	h;

	h = (t_hdochelper){0};
	h.s = s;
	h.rh = rh;
	h.delimiter = delimiter;
	if (!delimiter)
		return (ft_putstr_fd(ERR_HEREDOC_DELIMITER, STDERR_FILENO), 1);
	if (pipe(h.pipe_fds) == -1)
		return (perror("pipe"), FAILURE);
	h.pid = fork();
	if (h.pid == -1)
		return (perror("fork() heredoc"), close(h.pipe_fds[0]),
			close(h.pipe_fds[1]), FAILURE);
	if (h.pid == 0)
		heredoc_child_process(&h);
	else
		handle_main_process(&h);
	return (SUCCESS);
}
