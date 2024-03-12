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

#include "builtin.h"
#include "ft_printf/ft_printf.h"
#include "io_type.h"
#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int	update_fds(char *filename, t_redirecthelper *rh, bool fd_out)
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

static int	handle_redir(t_redirecthelper *rh, char **c_arr, t_shellstate *s,
		t_exechelper *h)
{
	if (ft_strcmp(c_arr[rh->i], ">") == 0 || ft_strcmp(c_arr[rh->i], ">>") == 0)
	{
		rh->flags = (O_WRONLY | O_CREAT | O_TRUNC);
		if (ft_strcmp(c_arr[rh->i], ">>") == 0)
			rh->flags = (O_WRONLY | O_CREAT | O_APPEND);
		rh->fd = open(c_arr[rh->i + 1], rh->flags, 0644);
		if (update_fds(c_arr[rh->i + 1], rh, true) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<") == 0)
	{
		rh->fd = open(c_arr[rh->i + 1], O_RDONLY);
		if (update_fds(c_arr[rh->i + 1], rh, false) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<<") == 0)
		handle_heredoc(rh, c_arr[rh->i + 1], s, h);
	return (SUCCESS);
}

static int	apply_fd_redirections(int last_out_fd, int last_in_fd)
{
	if (last_out_fd != -1)
	{
		if (dup2(last_out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 - STDOUT");
			close(last_out_fd);
			return (FAILURE);
		}
		close(last_out_fd);
	}
	if (last_in_fd != -1)
	{
		if (dup2(last_in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 - STDIN");
			close(last_in_fd);
			return (FAILURE);
		}
		close(last_in_fd);
	}
	return (SUCCESS);
}

int	handle_redirect(t_exechelper *eh, t_shellstate *s)
{
	t_redirecthelper	rh;
	t_command			command;

	command = (t_command){0};
	ft_memset(&rh, -1, sizeof(rh));
	rh.i = 0;
	while (eh->curr_cmd && eh->cmd_arr[rh.i])
	{
		if (eh->curr_cmd->next && eh->curr_cmd->next->ambiguous_redirect)
		{
			return (ft_fprintf(2, ERR_AMBIGUOUS_REDIRECT,
					eh->curr_cmd->next->content), FAILURE);
		}
		if (eh->curr_cmd->type != IO_NONE && handle_redir(&rh, eh->cmd_arr, s,
				eh) == FAILURE)
			return (FAILURE);
		eh->curr_cmd = eh->curr_cmd->next;
		rh.i++;
	}
	if (get_command(s->parsed_cmds[eh->i], &command) == FAILURE)
		exit(EXIT_FAILURE);
	free(eh->cmd_arr);
	vec_free(&command.io_vec);
	eh->cmd_arr = command.args;
	return (apply_fd_redirections(rh.last_out_fd, rh.last_in_fd));
}
