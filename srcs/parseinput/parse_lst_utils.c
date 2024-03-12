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
	if (!prev->is_quoted_redirector && (ft_strcmp(prev->content, ">") == 0
			|| ft_strcmp(prev->content, "<") == 0 || ft_strcmp(prev->content,
				">>") == 0))
	{
		return (true);
	}
	return (false);
}
