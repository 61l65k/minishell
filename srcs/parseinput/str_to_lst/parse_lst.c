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

static int	handle_non_quoted(t_lsthelper *lh)
{
	if (lh->arg_len > 0)
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
			if (assign_io_type(lh, ft_lstnew(lh->arg)) == FAILURE)
				return (free(lh->arg), ft_lstclear(&lh->head, free), FAILURE);
		}
	}
	lh->arg_start = lh->i + 1;
	return (SUCCESS);
}

static t_list	*allocate_lst(t_lsthelper *lh, t_shellstate *state)
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
					ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
			}
			else if (handle_non_quoted(lh) == FAILURE)
			{
				ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
			}
		}
		lh->i++;
	}
	return (lh->head);
}

t_list	*str_to_lst(const char *str, t_shellstate *s)
{
	t_lsthelper	lh;

	if (str == NULL)
		return (NULL);
	ft_memset(&lh, 0, sizeof(t_lsthelper));
	lh.start = str;
	lh.end = str + ft_strlen(str) - 1;
	while (*lh.start && (*lh.start == ' ' || *lh.start == '\t'))
		lh.start++;
	while (lh.end > lh.start && (*lh.end == ' ' || *lh.end == '\t'))
		lh.end--;
	lh.length = lh.end - lh.start + 1;
	return (allocate_lst(&lh, s));
}

void	ambigious_check(t_shellstate *s)
{
	t_list		**list;
	t_list		*tmp;
	const int	reset_e_len = ft_strlen(RESET_E);
	size_t		remaining_len;

	list = s->parsed_cmds;
	while (*list)
	{
		tmp = *list;
		while (tmp)
		{
			if (ft_strncmp(tmp->content, RESET_E, reset_e_len) == 0)
			{
				tmp->ambiguous_redirect = true;
				remaining_len = ft_strlen(tmp->content) - reset_e_len;
				ft_memmove(tmp->content, tmp->content + reset_e_len,
					remaining_len + 1);
				tmp->content[remaining_len] = '\0';
			}
			tmp = tmp->next;
		}
		list++;
	}
}
