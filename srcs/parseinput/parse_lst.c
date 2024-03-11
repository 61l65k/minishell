/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 02:37:28 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/05 02:37:30 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

static int	handle_quoted(t_lsthelper *t)
{
	if (!t->in_quote)
	{
		t->in_quote = 1;
		t->current_quote = t->start[t->i];
	}
	else
	{
		t->in_quote = false;
		t->arg_len = t->i - t->arg_start;
		t->arg = ft_strndup(t->start + t->arg_start, t->arg_len);
		if (!t->arg)
			return (ft_lstclear(&t->head, free), FAILURE);
		t->new_node = ft_lstnew(t->arg);
		if (!t->new_node)
			return (free(t->arg), ft_lstclear(&t->head, free), FAILURE);
		ft_isquotedredirector(t->new_node);
		if (!t->head)
			t->head = t->new_node;
		else
			t->current->next = t->new_node;
		t->current = t->new_node;
	}
	t->arg_start = ++t->i;
	return (SUCCESS);
}

static int	handle_wildcard(t_lsthelper *t)
{
	bool	found_match;

	found_match = false;
	while (true)
	{
		t->wcard.entry = readdir(t->wcard.dir);
		if (!t->wcard.entry)
			break ;
		if (t->wcard.entry->d_name[0] == '.' && (t->arg[0] != '.'
				&& t->arg[1] != '*'))
			continue ;
		if (wildcard_match(t->arg, t->wcard.entry->d_name))
		{
			create_add_node_wcard(t, t->wcard.entry->d_name);
			found_match = true;
		}
	}
	if (!found_match)
		create_add_node_wcard(t, t->arg);
	closedir(t->wcard.dir);
	free(t->arg);
	return (SUCCESS);
}

static int	handle_non_quoted(t_lsthelper *t)
{
	if (!t->in_quote && t->arg_len > 0)
	{
		t->arg = ft_strndup(t->start + t->arg_start, t->arg_len);
		if (!t->arg)
			return (ft_lstclear(&t->head, free), FAILURE);
		if (ft_strchr(t->arg, '*'))
		{
			t->wcard.dir = opendir(".");
			if (!t->wcard.dir || handle_wildcard(t) == FAILURE)
				return (free(t->arg), ft_lstclear(&t->head, free), FAILURE);
		}
		else
		{
			t->new_node = ft_lstnew(t->arg);
			if (!t->new_node)
				return (free(t->arg), ft_lstclear(&t->head, free), FAILURE);
			if (!t->head)
				t->head = t->new_node;
			else
				t->current->next = t->new_node;
			t->current = t->new_node;
		}
	}
	t->arg_start = t->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_lsthelper *t)
{
	while (t->i <= t->length)
	{
		if (need_handling(t, true))
		{
			if (handle_quoted(t))
				return (NULL);
			continue ;
		}
		if (need_handling(t, false))
		{
			if (!t->in_quote)
				t->arg_len = t->i - t->arg_start;
			else
				t->arg_len = t->i - t->arg_start + 1;
			if (t->is_adjacent)
			{
				if (handle_adjacent(t) == FAILURE)
					return (NULL);
			}
			else if (handle_non_quoted(t))
				return (NULL);
		}
		t->i++;
	}
	return (t->head);
}

t_list	*str_to_lst(const char *str)
{
	t_lsthelper	t;

	if (str == NULL)
		return (NULL);
	t = (t_lsthelper){0};
	t.start = str;
	t.end = str + ft_strlen(str) - 1;
	while (*t.start && (*t.start == ' ' || *t.start == '\t'))
		t.start++;
	while (t.end > t.start && (*t.end == ' ' || *t.end == '\t'))
		t.end--;
	t.length = t.end - t.start + 1;
	return (allocate_lst(&t));
}
