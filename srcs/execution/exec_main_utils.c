/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 04:15:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/11 04:15:21 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/**
 * @brief Checks the operators and skips the commands if needed.
 */
void	check_operators(t_exechelper *eh, t_shellstate *s)
{
	const t_list	*next_lst = s->parsed_cmds[eh->i + 1];

	if (next_lst)
	{
		if (next_lst && next_lst->op_type == OP_AND && s->last_exit_status != 0)
		{
			while (next_lst && next_lst->op_type != OP_OR)
				next_lst = s->parsed_cmds[eh->i++];
		}
		else if (next_lst && next_lst->op_type == OP_OR
			&& s->last_exit_status == 0)
		{
			while (next_lst && next_lst->op_type != OP_AND)
				next_lst = s->parsed_cmds[eh->i++];
		}
	}
	if (next_lst && next_lst->op_type != OP_NONE)
		eh->i++;
}

bool	check_pipedoc(t_shellstate *s, t_exechelper *eh)
{
	const t_list	*l = s->parsed_cmds[eh->i];
	const t_list	*next_lst = s->parsed_cmds[eh->i + 1];

	if (eh->i < s->cmd_count - 1 && next_lst->op_type == OP_PIPE)
	{
		while (l)
		{
			if (ft_strcmp(l->content, "<<") == 0)
			{
				eh->pipe_doc = true;
				return (true);
			}
			l = l->next;
		}
	}
	return (false);
}

bool	ambiguous_redirect(t_shellstate *s)
{
	const t_list	*cmd = s->parsed_cmds[0];

	while (cmd)
	{
		if (cmd->ambiguous_redirect == true)
		{
			ft_fprintf(2, ERR_AMBIGUOUS_REDIRECT, cmd->content);
			return (true);
		}
		cmd = cmd->next;
	}
	return (false);
}

int	calculate_subshell_offset(t_exechelper *eh, t_shellstate *s)
{
	int	offset;
	int	nested;

	offset = 0;
	nested = 0;
	while (s->parsed_cmds[eh->i + offset] != NULL)
	{
		if (s->parsed_cmds[eh->i + offset]->sub_type == SUB_START)
			nested++;
		if (s->parsed_cmds[eh->i + offset]->sub_type == SUB_END)
		{
			if (s->in_subshell && nested == 1)
				break ;
			nested--;
			if (!s->in_subshell && nested == 0)
				break ;
		}
		offset++;
	}
	return (offset + 1);
}

/**
 * @brief Duplicates the fd's forward if we are in a pipeline.
 * so next command can read from it.
 * & closes the pipefd if we are not in a pipeline.
 */
void	dup_forward_fd(t_shellstate *s, t_exechelper *eh)
{
	const t_list	*next_lst = s->parsed_cmds[eh->i + 1];

	if (eh->fd_in != 0)
	{
		dup2(eh->fd_in, STDIN_FILENO);
		close(eh->fd_in);
	}
	if (eh->i < s->cmd_count - 1 && next_lst->op_type == OP_PIPE)
	{
		close(eh->pipefd[0]);
		dup2(eh->pipefd[1], STDOUT_FILENO);
		close(eh->pipefd[1]);
	}
}
