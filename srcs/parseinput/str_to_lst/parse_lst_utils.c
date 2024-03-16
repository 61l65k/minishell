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

#include "libft.h"
#include "minishell.h"

t_operators	get_op_type(const char *str)
{
	if (!str)
		return (OP_NONE);
	if (ft_strcmp(str, "|") == 0)
		return (OP_PIPE);
	if (ft_strcmp(str, "&&") == 0)
		return (OP_AND);
	if (ft_strcmp(str, "||") == 0)
		return (OP_OR);
	return (OP_NONE);
}

int	assign_io_type(t_lsthelper *lh, t_list *new_node)
{
	if (!new_node)
		return (FAILURE);
	if (lh)
	{
		lh->new_node = new_node;
		if (!lh->head)
			lh->head = lh->new_node;
		else
			lh->current->next = lh->new_node;
		lh->current = lh->new_node;
	}
	new_node->op_type = get_op_type(new_node->content);
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

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

bool	is_spaces(const char *str)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (!ft_isspace(*str))
		{
			return (false);
		}
		str++;
	}
	return (true);
}
