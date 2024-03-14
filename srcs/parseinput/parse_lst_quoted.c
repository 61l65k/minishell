/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_quoted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 15:08:23 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 15:08:24 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/ft_printf.h"
#include "minishell.h"
#include "miniutils.h"

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

static int	process_quoted(t_lsthelper *t, t_adjacenthelper *ah)
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

int	handle_quoted(t_lsthelper *lh)
{
	t_adjacenthelper	ah;
	t_list				*new_node;

	ah = (t_adjacenthelper){0};
	ah.buff_size = 256;
	lh->i -= lh->arg_len;
	ah.buff = malloc(ah.buff_size);
	if (!ah.buff)
		return (FAILURE);
	ah.buffer_index = 0;
	process_quoted(lh, &ah);
	new_node = ft_lstnew(ah.buff);
	if (!new_node)
		return (free(ah.buff), FAILURE);
	if (!lh->head)
	{
		lh->head_assigned = true;
		lh->head = new_node;
	}
	else
		lh->current->next = new_node;
	lh->current = new_node;
	lh->arg_start = lh->i + 1;
	lh->in_quotes = false;
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
