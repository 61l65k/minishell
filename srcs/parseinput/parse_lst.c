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

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"

static int	handle_quoted(t_lsthelper *lh)
{
	if (!lh->in_quote)
	{
		lh->in_quote = 1;
		lh->current_quote = lh->start[lh->i];
	}
	else
	{
		lh->in_quote = false;
		lh->arg_len = lh->i - lh->arg_start;
		lh->arg = ft_strndup(lh->start + lh->arg_start, lh->arg_len);
		if (!lh->arg)
			return (ft_lstclear(&lh->head, free), FAILURE);
		lh->new_node = ft_lstnew(lh->arg);
		if (!lh->new_node)
			return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		ft_isquotedredirector(lh->new_node);
		if (!lh->head)
			lh->head = lh->new_node;
		else
			lh->current->next = lh->new_node;
		lh->current = lh->new_node;
	}
	lh->arg_start = ++lh->i;
	return (SUCCESS);
}

static int	handle_wildcard(t_lsthelper *lh, t_parsehelper *ph)
{
	lh->wcard.prev = lh->current;
	while (true)
	{
		lh->wcard.entry = readdir(lh->wcard.dir);
		if (!lh->wcard.entry)
			break ;
		if (lh->wcard.entry->d_name[0] == '.' && (lh->arg[0] != '.'
				&& lh->arg[1] != '*'))
			continue ;
		if (wildcard_match(lh->arg, lh->wcard.entry->d_name))
		{
			lh->wcard.match_count++;
			create_add_node_wcard(lh, lh->wcard.entry->d_name);
		}
	}
	if (lh->wcard.match_count > 1 && is_prev_redirector(lh->wcard.prev, ph))
	{
		ft_lstclear(&lh->wcard.prev->next, free);
		lh->current = lh->wcard.prev;
		create_add_node_wcard(lh, lh->arg);
		lh->current->ambigious_redirect = true;
	}
	if (!lh->wcard.match_count)
		create_add_node_wcard(lh, lh->arg);
	return (closedir(lh->wcard.dir), free(lh->arg), SUCCESS);
}

static int	handle_non_quoted(t_lsthelper *t, t_parsehelper *ph)
{
	if (!t->in_quote && t->arg_len > 0)
	{
		t->arg = ft_strndup(t->start + t->arg_start, t->arg_len);
		if (!t->arg)
			return (ft_lstclear(&t->head, free), FAILURE);
		if (ft_strchr(t->arg, '*'))
		{
			t->wcard.dir = opendir(".");
			if (!t->wcard.dir || handle_wildcard(t, ph) == FAILURE)
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

static t_list	*allocate_lst(t_lsthelper *lh, t_parsehelper *ph)
{
	while (lh->i <= lh->length)
	{
		if (need_handling(lh, true))
		{
			if (handle_quoted(lh))
				return (NULL);
			continue ;
		}
		if (need_handling(lh, false))
		{
			if (!lh->in_quote)
				lh->arg_len = lh->i - lh->arg_start;
			else
				lh->arg_len = lh->i - lh->arg_start + 1;
			if (lh->is_adjacent)
			{
				if (handle_adjacent(lh) == FAILURE)
					return (NULL);
			}
			else if (handle_non_quoted(lh, ph))
				return (NULL);
		}
		lh->i++;
	}
	return (lh->head);
}

t_list	*str_to_lst(const char *str, t_parsehelper *ph)
{
	t_lsthelper	lh;

	ph->ambigious_error = false;
	(void)ph;
	if (str == NULL)
		return (NULL);
	lh = (t_lsthelper){0};
	lh.start = str;
	lh.end = str + ft_strlen(str) - 1;
	while (*lh.start && (*lh.start == ' ' || *lh.start == '\t'))
		lh.start++;
	while (lh.end > lh.start && (*lh.end == ' ' || *lh.end == '\t'))
		lh.end--;
	lh.length = lh.end - lh.start + 1;
	return (allocate_lst(&lh, ph));
}
