/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirectors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:27:40 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 18:27:42 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

static int	redirect_fd(char *filename, t_redirecthelper *rh)
{
	if (rh->fd == -1)
	{
		perror(filename);
		if (rh->last_out_fd != -1)
			close(rh->last_out_fd);
		return (FAILURE);
	}
	if (rh->last_out_fd != -1)
		close(rh->last_out_fd);
	rh->last_out_fd = rh->fd;
	return (SUCCESS);
}

static int	check_heredoc(t_redirecthelper *rh, char *delimiter)
{
	(void)rh;
	(void)delimiter;
	return (SUCCESS);
}

static int	check_operator(t_redirecthelper *rh, char **c_arr)
{
	if (ft_strcmp(c_arr[rh->i], ">") == 0 || ft_strcmp(c_arr[rh->i], ">>") == 0)
	{
		rh->flags = (O_WRONLY | O_CREAT | O_TRUNC);
		if (ft_strcmp(c_arr[rh->i], ">>") == 0)
			rh->flags = (O_WRONLY | O_CREAT | O_APPEND);
		rh->fd = open(c_arr[rh->i + 1], rh->flags, 0644);
		if (redirect_fd(c_arr[rh->i++ + 1], rh) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<") == 0)
	{
		rh->fd = open(c_arr[rh->i + 1], O_RDONLY);
		if (redirect_fd(c_arr[rh->i++ + 1], rh) == FAILURE)
			return (FAILURE);
	}
	else if (ft_strcmp(c_arr[rh->i], "<<") == 0)
	{
		if (check_heredoc(rh, c_arr[rh->i++ + 1]) == FAILURE)
			return (FAILURE);
	}
	else
		c_arr[rh->j++] = c_arr[rh->i];
	return (SUCCESS);
}

int	apply_command_redirections(char **cmd_arr)
{
	t_redirecthelper	rh;

	ft_memset(&rh, -1, sizeof(rh));
	rh.i = 0;
	rh.j = 0;
	while (cmd_arr[rh.i])
	{
		if (check_operator(&rh, cmd_arr) == FAILURE)
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
