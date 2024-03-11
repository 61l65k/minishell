/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_adj.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 15:08:23 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 15:08:24 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_buffer_if_needed(t_adjacenthelper *ah)
{
	if (ah->buffer_index >= ah->buff_size - 1)
	{
		ah->new_buff = ft_realloc(ah->buff, ah->buff_size, ah->buff_size * 2);
		if (!ah->new_buff)
			return (free(ah->buff), FAILURE);
		ah->buff = ah->new_buff;
		ah->buff_size *= 2;
	}
	return (SUCCESS);
}

static int	process_adjacent(t_lsthelper *t, t_adjacenthelper *ah)
{
	while (t->i <= t->length && (ah->current_quote || t->start[t->i] != ' '))
	{
		if (expand_buffer_if_needed(ah) == FAILURE)
			return (FAILURE);
		if ((t->start[t->i] == '\'' || t->start[t->i] == '"')
			&& !ah->current_quote)
		{
			ah->current_quote = t->start[t->i++];
			continue ;
		}
		else if (t->start[t->i] == ah->current_quote)
		{
			ah->current_quote = 0;
			t->i++;
			if (t->i < t->length && t->start[t->i] != ' ')
				continue ;
			else
				break ;
		}
		ah->buff[ah->buffer_index++] = t->start[t->i++];
	}
	ah->buff[ah->buffer_index] = '\0';
	return (SUCCESS);
}

int	handle_adjacent(t_lsthelper *t)
{
	t_adjacenthelper	ah;

	ah = (t_adjacenthelper){0};
	ah.buff_size = 256;
	t->i -= t->arg_len;
	ah.buff = malloc(ah.buff_size);
	if (!ah.buff)
		return (FAILURE);
	ah.buffer_index = 0;
	process_adjacent(t, &ah);
	t->arg = ah.buff;
	t->new_node = ft_lstnew(t->arg);
	if (!t->new_node)
		return (free(ah.buff), FAILURE);
	if (!t->head)
		t->head = t->new_node;
	else
		t->current->next = t->new_node;
	t->current = t->new_node;
	t->arg_start = t->i + 1;
	t->is_adjacent = false;
	return (SUCCESS);
}

bool	is_spaces(const char *str)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (!isspace((unsigned char)*str))
		{
			return (false);
		}
		str++;
	}
	return (true);
}
