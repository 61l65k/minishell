/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_sub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 07:52:51 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/14 07:52:54 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_type.h"
#include "minishell.h"

int	create_head_node(t_lsthelper *lh)
{
	t_list	*head_node;

	head_node = ft_lstnew(NULL);
	if (!lh->head)
	{
		lh->head_assigned = true;
		if (!head_node)
			return (-1);
		lh->head = head_node;
		lh->current = head_node;
	}
	return (0);
}

int	start_sublist(t_lsthelper *lh)
{
	const char	*trimmed_content = ft_strndup(lh->arg + 1, lh->arg_len - 1);
	t_list		*new_node;

	lh->sub_depth++;
	new_node = ft_lstnew((char *)trimmed_content);
	free(lh->arg);
	if (!new_node)
		return (-1);
	create_head_node(lh);
	if (lh->current)
	{
		lh->current->subshell = new_node;
		new_node->parent = lh->current;
		new_node->type = get_io_type(new_node->content);
		lh->current_parent = new_node->parent;
		lh->current = new_node;
	}
	return (0);
}

int	end_sublist(t_lsthelper *lh)
{
	const char	*trimmed_content = ft_strndup(lh->arg, lh->arg_len - 1);
	t_list		*new_node;

	lh->sub_depth--;
	new_node = ft_lstnew((char *)trimmed_content);
	free(lh->arg);
	if (!new_node)
		return (-1);
	new_node->type = get_io_type(new_node->content);
	lh->current->next = new_node;
	new_node->parent = lh->current_parent;
	if (ft_strchr(trimmed_content, '*'))
	{
		lh->arg = ft_strdup(trimmed_content);
		lh->wcard.dir = opendir(".");
		if (!lh->wcard.dir || handle_wildcard(lh) == FAILURE)
			return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
	}
	lh->current = lh->current_parent;
	return (0);
}
