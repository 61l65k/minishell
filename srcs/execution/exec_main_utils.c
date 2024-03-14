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

#include "minishell.h"

/**
 * @brief Checks the operators and skips the commands if needed.
 */
void	check_operators(t_exechelper *h, t_shellstate *s)
{
	if (h->i < s->operator_count)
	{
		if (s->operators[h->i] == OP_AND && s->last_exit_status != 0)
		{
			while (h->i < s->operator_count && s->operators[h->i] != OP_OR)
				h->i++;
		}
		else if (s->operators[h->i] == OP_OR && s->last_exit_status == 0)
		{
			while (h->i < s->operator_count && s->operators[h->i] != OP_AND)
				h->i++;
		}
	}
}

bool	check_pipedoc(t_shellstate *s, t_exechelper *h)
{
	const t_list	*l = s->parsed_cmds[h->i];

	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		while (l)
		{
			if (ft_strcmp(l->content, "<<") == 0)
			{
				h->pipe_doc = true;
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
