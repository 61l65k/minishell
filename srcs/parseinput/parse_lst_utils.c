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
		}
		else
			free(node_data);
	}
}

bool	need_handling(t_lsthelper *t, bool check_quoted)
{
	const bool	is_quote = t->start[t->i] == '\'' || t->start[t->i] == '"';

	if (check_quoted)
	{
		if (is_quote)
		{
			t->is_adjacent = true;
			return (!t->is_adjacent);
		}
	}
	else
	{
		return ((t->start[t->i] == ' ') || t->i == t->length || t->is_adjacent);
	}
	return (false);
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
