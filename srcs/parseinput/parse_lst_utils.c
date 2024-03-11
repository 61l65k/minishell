/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 03:38:52 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 03:38:54 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

void	create_add_node_wcard(t_lsthelper *t, char *data)
{
	char	*node_data;
	t_list	*new_node;

	node_data = ft_strdup(data);
	if (node_data != NULL)
	{
		new_node = ft_lstnew(node_data);
		if (new_node != NULL)
		{
			if (!t->head)
				t->head = new_node;
			else
				t->current->next = new_node;
			t->current = new_node;
		}
		else
			free(node_data);
	}
}

bool	need_handling(t_lsthelper *t, bool check_quoted)
{
	if (check_quoted)
	{
		if ((t->start[t->i] == '\'' || t->start[t->i] == '"') && (!t->in_quote
				|| t->start[t->i] == t->current_quote))
		{
			if (!t->in_quote || (t->start[t->i] == t->current_quote
					&& t->start[t->i + 1] && t->start[t->i + 1] != ' '))
			{
				t->is_adjacent = true;
				return (0);
			}
			return (1);
		}
	}
	else
	{
		if ((!t->in_quote && t->start[t->i] == ' ') || t->i == t->length
			|| t->is_adjacent)
			return (1);
	}
	return (0);
}

bool	need_handling_non_quoted(t_lsthelper *t)
{
	if ((!t->in_quote && t->start[t->i] == ' ') || t->i == t->length
		|| t->is_adjacent)
		return (1);
	return (0);
}
