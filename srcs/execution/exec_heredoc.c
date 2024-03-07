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

#include "minishell.h"
#include <linux/limits.h>
#include <sys/types.h>

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
		_exit(EXIT_SUCCESS);
}

static void	heredoc_child_process(int pipe_fds[2], char *delimiter)
{
	char	*line;

	signal(SIGINT, heredoc_signal_handler);
	close(pipe_fds[0]);
	while (true)
	{
		line = readline("heredoc> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
			break ;
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
	close(pipe_fds[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_main_process(t_redirecthelper *rh, int pipe_fds[2],
		t_shellstate *s, pid_t pid)
{
	sigaction(SIGINT, &s->ignoreaction, &s->sigaction);
	s->in_heredoc = true;
	close(pipe_fds[1]);
	waitpid(pid, NULL, 0);
	sigaction(SIGINT, &s->sigaction, NULL);
	s->in_heredoc = false;
	rh->fd = pipe_fds[0];
	redirect_fd(NULL, rh, false);
}

int	handle_heredoc(t_redirecthelper *rh, char *delimiter, t_shellstate *s)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), FAILURE);
	pid = fork();
	if (pid == -1)
		return (perror("fork() heredoc"), close(pipe_fds[0]),
			close(pipe_fds[1]), FAILURE);
	if (pid == 0)
		heredoc_child_process(pipe_fds, delimiter);
	else
		handle_main_process(rh, pipe_fds, s, pid);
	return (SUCCESS);
}
