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

int	update_fds(char *filename, t_redirecthelper *rh, bool fd_out)
{
	if (rh->fd == -1 && filename)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: %s\n", filename, strerror(errno));
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

static int	redir(t_redirecthelper *rh, t_exechelper *eh)
{
	const t_list	*node = eh->curr_cmd;

	if (node->type == IO_OUT_TRUNC || node->type == IO_OUT_APPEND)
	{
		rh->flags = (O_WRONLY | O_CREAT | O_TRUNC);
		if (node->type == IO_OUT_APPEND)
			rh->flags = (O_WRONLY | O_CREAT | O_APPEND);
		rh->fd = open(node->next->content, rh->flags, 0644);
		if (update_fds(node->next->content, rh, true) == FAILURE)
			return (FAILURE);
	}
	else if (node->type == IO_INPUT)
	{
		rh->fd = open(node->next->content, O_RDONLY);
		if (update_fds(node->next->content, rh, false) == FAILURE)
			return (FAILURE);
	}
	else if (node->type == IO_HEREDOC)
		handle_heredoc(rh, node->next->content, eh);
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

int	handle_redirect(t_exechelper *eh)
{
	t_redirecthelper	rh;

	ft_memset(&rh, -1, sizeof(rh));
	while (eh->curr_cmd)
	{
		if (eh->curr_cmd->next && eh->curr_cmd->next->ambiguous_redirect \
			&& eh->curr_cmd->type != IO_HEREDOC)
		{
			return (ft_fprintf(2, ERR_AMBIGUOUS_REDIRECT,
					eh->curr_cmd->next->content), FAILURE);
		}
		if (eh->curr_cmd->type != IO_NONE && redir(&rh, eh))
			return (FAILURE);
		eh->curr_cmd = eh->curr_cmd->next;
	}
	return (apply_fd_redirections(rh.last_out_fd, rh.last_in_fd));
}
