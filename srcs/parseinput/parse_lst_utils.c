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

#include "minishell.h"
#include "miniutils.h"

bool	need_handling(t_lsthelper *t, bool check_quoted)
{
	if (check_quoted)
	{
		if ((t->start[t->i] == '\'' || t->start[t->i] == '"') && (!t->in_quote
				|| t->start[t->i] == t->current_quote))
		{
			if (!t->in_quote && t->start[t->i - 1] != ' ')
			{
				t->is_adjacted = true;
				return (0);
			}
			return (1);
		}
	}
	else
	{
		if ((!t->in_quote && t->start[t->i] == ' ') || t->i == t->length
			|| t->is_adjacted)
			return (1);
	}
	return (0);
}

bool	need_handling_non_quoted(t_lsthelper *t)
{
	if ((!t->in_quote && t->start[t->i] == ' ') || t->i == t->length
		|| t->is_adjacted)
		return (1);
	return (0);
}

int	handle_adjacted(t_lsthelper *t)
{
	char	*buffer;
	size_t	buffer_index;
	size_t	buffer_size;
	char	current_quote;
	char	*new_buffer;

	buffer_size = 256;
	current_quote = 0;
	t->i -= t->arg_len;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (FAILURE);
	buffer_index = 0;
	while (t->i <= t->length && (current_quote || t->start[t->i] != ' '))
	{
		if (buffer_index >= buffer_size - 1)
		{
			new_buffer = ft_realloc(buffer, buffer_size, buffer_size * 2);
			if (!new_buffer)
				return (free(buffer), FAILURE);
			buffer = new_buffer;
			buffer_size *= 2;
		}
		if ((t->start[t->i] == '\'' || t->start[t->i] == '"') && !current_quote)
		{
			current_quote = t->start[t->i++];
			continue ;
		}
		else if (t->start[t->i] == current_quote)
		{
			current_quote = 0;
			t->i++;
			if (t->i < t->length && t->start[t->i] != ' ')
				continue ;
			else
				break ;
		}
		buffer[buffer_index++] = t->start[t->i++];
	}
	buffer[buffer_index] = '\0';
	t->arg = buffer;
	t->new_node = ft_lstnew(t->arg);
	if (!t->new_node)
		return (free(buffer), FAILURE);
	if (!t->head)
		t->head = t->new_node;
	else
		t->current->next = t->new_node;
	t->current = t->new_node;
	t->arg_start = t->i + 1;
	t->is_adjacted = false;
	return (SUCCESS);
}
