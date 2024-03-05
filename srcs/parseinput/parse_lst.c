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

static int	handle_quoted(t_trimhelper *t)
{
	if (!t->in_quote)
	{
		t->in_quote = 1;
		t->current_quote = t->start[t->i];
		t->arg_start = t->i + 1;
	}
	else
	{
		t->in_quote = 0;
		t->arg_len = t->i - t->arg_start;
		t->arg = ft_strndup(t->start + t->arg_start, t->arg_len);
		if (!t->arg)
			return (ft_lstclear(&t->head, free), FAILURE);
		t->new_node = ft_lstnew(t->arg);
		if (!t->new_node)
			return (free(t->arg), ft_lstclear(&t->head, free), FAILURE);
		if (!t->head)
			t->head = t->new_node;
		else
			t->current->next = t->new_node;
		t->current = t->new_node;
		t->arg_start = t->i + 1;
	}
	t->i++;
	return (SUCCESS);
}

static int	handle_non_quoted(t_trimhelper *t)
{
	if (!t->in_quote)
		t->arg_len = t->i - t->arg_start;
	else
		t->arg_len = t->i - t->arg_start + 1;
	if (t->arg_len > 0)
	{
		t->arg = ft_strndup(t->start + t->arg_start, t->arg_len);
		if (!t->arg)
			return (ft_lstclear(&t->head, free), FAILURE);
		t->new_node = ft_lstnew(t->arg);
		if (!t->new_node)
			return (free(t->arg), ft_lstclear(&t->head, free), FAILURE);
		if (!t->head)
			t->head = t->new_node;
		else
			t->current->next = t->new_node;
		t->current = t->new_node;
	}
	t->arg_start = t->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_trimhelper *t)
{
	while (t->i <= t->length)
	{
		if ((t->start[t->i] == '\'' || t->start[t->i] == '"') && (!t->in_quote
				|| t->start[t->i] == t->current_quote))
		{
			if (handle_quoted(t))
				return (NULL);
			continue ;
		}
		if ((!t->in_quote && t->start[t->i] == ' ') || t->i == t->length)
		{
			if (handle_non_quoted(t))
				return (NULL);
		}
		t->i++;
	}
	return (t->head);
}

t_list	*str_to_lst(const char *str)
{
	t_trimhelper	t;

	if (str == NULL)
		return (NULL);
	ft_memset(&t, 0, sizeof(t_trimhelper));
	t.start = str;
	t.end = str + ft_strlen(str) - 1;
	while (*t.start && (*t.start == ' ' || *t.start == '\t'))
		t.start++;
	while (t.end > t.start && (*t.end == ' ' || *t.end == '\t'))
		t.end--;
	t.length = t.end - t.start + 1;
	return (allocate_lst(&t));
}
