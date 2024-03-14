/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_wcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:11:19 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/14 11:11:20 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_add_node_wcard(t_lsthelper *lh, char *data)
{
	char	*node_data;
	t_list	*new_node;

	node_data = ft_strdup(data);
	if (node_data == NULL)
		return (FAILURE);
	new_node = ft_lstnew(node_data);
	if (new_node == NULL)
		return (free(node_data), FAILURE);
	if (!lh->head)
		lh->head = new_node;
	else
		lh->current->next = new_node;
	lh->current = new_node;
	return (SUCCESS);
}

static bool	wildcard_match(const char *pattern, const char *str)
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

static int	process_wcard_dir_entries(t_lsthelper *lh)
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
			if (create_add_node_wcard(lh, lh->wcard.entry->d_name) != SUCCESS)
				return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	handle_wildcard(t_lsthelper *lh)
{
	lh->wcard.prev = lh->current;
	if (process_wcard_dir_entries(lh) != SUCCESS)
		return (closedir(lh->wcard.dir), free(lh->arg), FAILURE);
	if (lh->wcard.match_count > 1 && is_prev_redirector(lh->wcard.prev))
	{
		ft_lstclear(&lh->wcard.prev->next, free);
		lh->current = lh->wcard.prev;
		if (create_add_node_wcard(lh, lh->arg) != SUCCESS)
			return (closedir(lh->wcard.dir), free(lh->arg), FAILURE);
		lh->current->ambiguous_redirect = \
		get_io_type(lh->wcard.prev->content) != IO_HEREDOC;
	}
	if (!lh->wcard.match_count && create_add_node_wcard(lh, lh->arg) != SUCCESS)
		return (closedir(lh->wcard.dir), free(lh->arg), FAILURE);
	return (closedir(lh->wcard.dir), free(lh->arg), SUCCESS);
}
