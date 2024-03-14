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

int	assign_io_type(t_lsthelper *lh, t_list *new_node)
{
	if (!new_node)
		return (FAILURE);
	lh->new_node = new_node;
	if (!lh->head)
		lh->head = lh->new_node;
	else
		lh->current->next = lh->new_node;
	lh->current = lh->new_node;
	new_node->type = get_io_type(new_node->content);
	return (SUCCESS);
}

bool	is_prev_redirector(const t_list *prev)
{
	if (prev == NULL)
		return (false);
	if (get_io_type(prev->content))
	{
		return (true);
	}
	return (false);
}

bool	is_spaces(const char *str)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (!isspace((unsigned char)*str))
		{
			return (false);
		}
		str++;
	}
	return (true);
}
