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

#include "io_type.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"

void	create_add_node_wcard(t_lsthelper *lh, char *data)
{
	char	*node_data;
	t_list	*new_node;

	node_data = ft_strdup(data);
	if (node_data != NULL)
	{
		new_node = ft_lstnew(node_data);
		if (new_node != NULL)
		{
			if (!lh->head)
				lh->head = new_node;
			else
				lh->current->next = new_node;
			lh->current = new_node;
			new_node->type = get_io_type(new_node->content);
			new_node->parent = lh->current_parent;
		}
		else
			free(node_data);
	}
}

int	assign_io_type(t_lsthelper *lh, t_list *new_node)
{
	if (!new_node)
		return (FAILURE);
	if (!lh->head)
	{
		lh->head_assigned = true;
		lh->head = new_node;
	}
	else
		lh->current->next = new_node;
	lh->current = new_node;
	new_node->type = get_io_type(new_node->content);
	new_node->parent = lh->current_parent;
	return (SUCCESS);
}

/**
 * @brief Matches the pattern with the string.
 */
bool	wildcard_match(const char *pattern, const char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		if (wildcard_match(pattern + 1, str))
			return (true);
		if (*str && wildcard_match(pattern, str + 1))
			return (true);
	}
	else if (*pattern == *str || (*pattern == '?' && *str))
	{
		return (wildcard_match(pattern + 1, str + 1));
	}
	return (false);
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

void	print_list(t_list *node, int depth)
{
	while (node)
	{
		printf("%*scontent: %s, parent: %p, subshell: %p\n", depth * 4, "",
			node->content ? node->content : "NULL", node->parent,
			node->subshell);
		if (node->subshell)
		{
			print_list(node->subshell, depth + 1);
		}
		node = node->next;
	}
}