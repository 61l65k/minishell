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
#include <readline/history.h>

static void	process_wcard_dir_entries(t_lsthelper *lh)
{
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
}

static int	handle_wildcard(t_lsthelper *lh)
{
	lh->wcard.prev = lh->current;
	process_wcard_dir_entries(lh);
	if (lh->wcard.match_count > 1 && is_prev_redirector(lh->wcard.prev))
	{
		ft_lstclear(&lh->wcard.prev->next, free);
		lh->current = lh->wcard.prev;
		create_add_node_wcard(lh, lh->arg);
		lh->current->ambiguous_redirect = get_io_type(lh->wcard.prev->content) != IO_IN_HEREDOC;
	}
	if (!lh->wcard.match_count)
		create_add_node_wcard(lh, lh->arg);
	return (closedir(lh->wcard.dir), free(lh->arg), SUCCESS);
}

void	start_sublist(t_lsthelper *lh)
{
	char	*trimmed_content;
	t_list	*new_node;
	t_list	*head_node;

	//	printf("start lh arg %s\n", lh->arg);
	trimmed_content = ft_strndup(lh->arg + 1, lh->arg_len - 1);
	printf("trimmed start lh arg %s\n", trimmed_content);
	new_node = ft_lstnew(trimmed_content);
	free(lh->arg);
	if (!new_node)
		return ;
	if (!lh->head)
	{
		head_node = ft_lstnew(NULL);
		if (!head_node)
		{
			free(new_node);
			return ;
		}
		lh->head = head_node;
		lh->current = head_node;
	}
	if (lh->current)
	{
		lh->current->subshell = new_node;
		new_node->parent = lh->current;
	}
	lh->current_parent = new_node->parent;
	lh->current = new_node;
}

void	end_sublist(t_lsthelper *lh)
{
	char	*trimmed_content;
	t_list	*new_node;

	// printf("end sub lh arg %s\n", lh->arg);
	trimmed_content = ft_strndup(lh->arg, lh->arg_len - 1);
	printf("trimmed end lh arg %s\n", trimmed_content);
	new_node = ft_lstnew(trimmed_content);
	free(lh->arg);
	if (!new_node)
		return ;
	if (lh->current)
	{
		lh->current->next = new_node;
		new_node->parent = lh->current_parent;
	}
	lh->current = lh->current_parent;
}

static int	handle_non_quoted(t_lsthelper *lh)
{
	if (lh->arg_len > 0)
	{
		lh->arg = ft_strndup(lh->start + lh->arg_start, lh->arg_len);
		if (!lh->arg)
			return (ft_lstclear(&lh->head, free), FAILURE);
		if (lh->arg[0] == '(')
			start_sublist(lh);
		else if (lh->arg[ft_strlen(lh->arg) - 1] == ')')
			end_sublist(lh);
		else if (ft_strchr(lh->arg, '*'))
		{
			lh->wcard.dir = opendir(".");
			if (!lh->wcard.dir || handle_wildcard(lh) == FAILURE)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		}
		else
		{
			// printf("assignin start lh arg %s\n", lh->arg);
			if (assign_io_type(lh, ft_lstnew(lh->arg)) == FAILURE)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		}
		// printf("current content: %s\n", lh->current->content);
		// if (lh->current_parent)
		//		printf("parent content: %s\n", lh->current_parent->content);
	}
	lh->arg_start = lh->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_lsthelper *lh)
{
	while (lh->i <= lh->length)
	{
		if (lh->start[lh->i] == '\'' || lh->start[lh->i] == '"')
		{
			lh->in_quotes = true;
			lh->i++;
		}
		if (((lh->start[lh->i] == ' ' || lh->i == lh->length)) || lh->in_quotes)
		{
			lh->arg_len = lh->i - lh->arg_start;
			if (lh->in_quotes)
			{
				if (handle_quoted(lh) == FAILURE)
				{
					printf("failed to handle quoted\n");
					return (NULL);
				}
			}
			else if (handle_non_quoted(lh))
			{
				printf("failed to handle non quoted\n");
				return (NULL);
			}
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
