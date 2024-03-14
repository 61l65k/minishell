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
			if (assign_io_type(lh, ft_lstnew(lh->arg)) == FAILURE)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		}
	}
	lh->arg_start = lh->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_lsthelper *lh)
{
	printf("START lh->start %s\n", lh->start);
	while (lh->i <= lh->length)
	{
		if (lh->start[lh->i] == '\'' || lh->start[lh->i - lh->arg_len] == '"')
		{
			lh->in_quotes = true;
			lh->i++;
		}
		if (((lh->start[lh->i] == ' ' || lh->i == lh->length)) || lh->in_quotes)
		{
			lh->arg_len = lh->i - lh->arg_start;
			printf("arg start pos %s\n", &lh->start[lh->i - lh->arg_len]);
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
	printf("EXITING HEAD CONTENT: %s\n", lh->head->content);
	return (lh->head);
}

int	str_to_lst(const char *str, t_lsthelper *lh, t_shellstate *s, int i)
{
	t_list	*temp;

	if (!str)
		return (FAILURE);
	lh->i = 0;
	lh->start = str;
	lh->end = str + ft_strlen(str) - 1;
	lh->arg_start = 0;
	lh->head_assigned = false;
	while (*lh->start && (*lh->start == ' ' || *lh->start == '\t'))
		lh->start++;
	while (lh->end > lh->start && (*lh->end == ' ' || *lh->end == '\t'))
		lh->end--;
	lh->length = lh->end - lh->start + 1;
	temp = allocate_lst(lh);
	if (lh->head_assigned)
		s->parsed_cmds[i] = temp;
	else
		s->parsed_cmds[i] = NULL;
	return (SUCCESS);
}
