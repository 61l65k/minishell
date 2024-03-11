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

/***
 * @brief Checks if we need to
 */
bool	check_pipedoc(t_shellstate *s, t_exechelper *h)
{
	const t_list	*l = s->parsed_cmds[h->i];

	if (h->i < s->cmd_count - 1 && s->operators[h->i] == OP_PIPE)
	{
		while (l)
		{
			if (!l->is_quoted_redirector && ft_strcmp(l->content, "<<") == 0)
			{
				h->pipe_doc = true;
				return (true);
			}
			l = l->next;
		}
	}
	return (false);
}
