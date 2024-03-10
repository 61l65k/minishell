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

#include "libft.h"
#include "minishell.h"
#include "miniutils.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

int	redirect_fd(char *filename, t_redirecthelper *rh, bool fd_out)
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

static void	shift_array_elements(char **c_arr, t_redirecthelper *rh)
{
	c_arr[rh->j++] = c_arr[rh->i];
}

static int	handle_redir(t_redirecthelper *rh, char **c_arr, t_shellstate *s)
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
		handle_heredoc(rh, c_arr[rh->i++ + 1], s);
	else
		shift_array_elements(c_arr, rh);
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

int	apply_cmd_redirections(t_exechelper *h, t_shellstate *s,
		const t_list *curr_cmd)
{
	t_redirecthelper	rh;

	ft_memset(&rh, -1, sizeof(rh));
	rh.j = 0;
	rh.i = 0;
	while (curr_cmd && h->cmd_arr[rh.i])
	{
		if (curr_cmd->is_quoted_redirector == false && handle_redir(&rh,
				h->cmd_arr, s) == FAILURE)
			return (FAILURE);
		curr_cmd = curr_cmd->next;
		rh.i++;
	}
	if (rh.j != 0)
		h->cmd_arr[rh.j] = NULL;
	return (apply_fd_redirections(rh.last_out_fd, rh.last_in_fd));
}
