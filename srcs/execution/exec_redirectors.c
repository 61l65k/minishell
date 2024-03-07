/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirectors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:27:40 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 21:27:45 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

static int	redirect_fd(char *filename, t_redirecthelper *rh, bool fd_out)
{
	if (rh->fd == -1 && filename)
	{
		perror(filename);
		if (rh->last_out_fd != -1)
			close(rh->last_out_fd);
		return (FAILURE);
	}
	if (fd_out)
	{
		if (rh->last_out_fd != -1)
			close(rh->last_out_fd);
		rh->last_out_fd = rh->fd;
	}
	else
	{
		if (rh->last_in_fd != -1)
			close(rh->last_in_fd);
		rh->last_in_fd = rh->fd;
	}
	return (SUCCESS);
}

static int	handle_heredoc(t_redirecthelper *rh, char *delimiter,
		t_shellstate *s)
{
	int		pipe_fds[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), FAILURE);
	pid = fork();
	if (pid == -1)
		return (perror("fork() heredoc"), close(pipe_fds[0]),
			close(pipe_fds[1]), FAILURE);
	if (pid == 0)
	{
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
	else
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
	return (SUCCESS);
}

static int	check_operator(t_redirecthelper *rh, char **c_arr, t_shellstate *s)
{
	if (ft_strcmp(c_arr[rh->i], ">") == 0 || ft_strcmp(c_arr[rh->i], ">>") == 0)
	{
		rh->flags = (O_WRONLY | O_CREAT | O_TRUNC);
		if (ft_strcmp(c_arr[rh->i], ">>") == 0)
			rh->flags = (O_WRONLY | O_CREAT | O_APPEND);
		rh->fd = open(c_arr[rh->i + 1], rh->flags, 0644);
		if (redirect_fd(c_arr[rh->i++ + 1], rh, true) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<") == 0)
	{
		rh->fd = open(c_arr[rh->i + 1], O_RDONLY);
		if (redirect_fd(c_arr[rh->i++ + 1], rh, false) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<<") == 0)
	{
		if (handle_heredoc(rh, c_arr[rh->i++ + 1], s) == FAILURE)
			return (FAILURE);
	}
	else
		c_arr[rh->j++] = c_arr[rh->i];
	return (SUCCESS);
}

int	apply_cmd_redirections(char **cmd_arr, t_shellstate *s)
{
	t_redirecthelper	rh;

	ft_memset(&rh, -1, sizeof(rh));
	rh.i = 0;
	rh.j = 0;
	while (cmd_arr[rh.i])
	{
		if (check_operator(&rh, cmd_arr, s) == FAILURE)
			return (FAILURE);
		rh.i++;
	}
	cmd_arr[rh.j] = NULL;
	if (rh.last_out_fd != -1)
	{
		if (dup2(rh.last_out_fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), close(rh.last_out_fd), FAILURE);
		close(rh.last_out_fd);
	}
	if (rh.last_in_fd != -1)
	{
		if (dup2(rh.last_in_fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(rh.last_in_fd), FAILURE);
		close(rh.last_in_fd);
	}
	return (SUCCESS);
}
