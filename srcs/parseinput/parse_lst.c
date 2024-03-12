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

#include "io_type.h"
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

static int	handle_wildcard(t_lsthelper *lh)
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
	if (lh->wcard.match_count > 1 && is_prev_redirector(lh->wcard.prev))
	{
		ft_lstclear(&lh->wcard.prev->next, free);
		lh->current = lh->wcard.prev;
		create_add_node_wcard(lh, lh->arg);
		lh->current->ambiguous_redirect = get_io_type(lh->wcard.prev->content) != IO_IN_HEREDOC;
	}
	if (lh->wcard.prev && !ft_strcmp(lh->wcard.prev->content, "rm")
		&& confirm_rm(lh))
		return (ft_lstclear(&lh->head, free), FAILURE);
	if (!lh->wcard.match_count)
		create_add_node_wcard(lh, lh->arg);
	return (closedir(lh->wcard.dir), free(lh->arg), SUCCESS);
}

static int	handle_non_quoted(t_lsthelper *lh)
{
	if (!lh->in_quote && lh->arg_len > 0)
	{
		lh->arg = ft_strndup(lh->start + lh->arg_start, lh->arg_len);
		if (!lh->arg)
			return (ft_lstclear(&lh->head, free), FAILURE);
		if (ft_strchr(lh->arg, '*'))
		{
			lh->wcard.dir = opendir(".");
			if (!lh->wcard.dir || handle_wildcard(lh) == FAILURE)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		}
		else
		{
			lh->new_node = ft_lstnew(lh->arg);
			if (!lh->new_node)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
			if (!lh->head)
				lh->head = lh->new_node;
			else
				lh->current->next = lh->new_node;
			lh->current = lh->new_node;
		}
	}
	lh->arg_start = lh->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_lsthelper *lh)
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
			else if (handle_non_quoted(lh))
				return (NULL);
		}
		lh->i++;
	}
	return (lh->head);
}

t_list	*str_to_lst(const char *str, t_parsehelper *ph)
{
	t_lsthelper	lh;

	if (str == NULL)
		return (NULL);
	lh = (t_lsthelper){0};
	lh.ph = ph;
	lh.start = str;
	lh.end = str + ft_strlen(str) - 1;
	while (*lh.start && (*lh.start == ' ' || *lh.start == '\t'))
		lh.start++;
	while (lh.end > lh.start && (*lh.end == ' ' || *lh.end == '\t'))
		lh.end--;
	lh.length = lh.end - lh.start + 1;
	return (allocate_lst(&lh));
}
