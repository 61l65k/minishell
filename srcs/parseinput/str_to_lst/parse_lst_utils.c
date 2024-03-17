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

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"

t_operators	get_op_type(const char *str, bool use_strstr)
{
	if (!str)
		return (OP_NONE);
	if (use_strstr)
	{
		if (ft_strnstr(str, "|", ft_strlen(str)))
			return (OP_PIPE);
		if (ft_strnstr(str, "&&", ft_strlen(str)))
			return (OP_AND);
		if (ft_strnstr(str, "||", ft_strlen(str)))
			return (OP_OR);
	}
	else
	{
		if (ft_strcmp(str, "|") == 0)
			return (OP_PIPE);
		if (ft_strcmp(str, "&&") == 0)
			return (OP_AND);
		if (ft_strcmp(str, "||") == 0)
			return (OP_OR);
	}
	return (OP_NONE);
}

int	assign_node_types(t_lsthelper *lh, t_list *new_node, t_subtype subtype)
{
	if (!new_node)
		return (FAILURE);
	if (lh)
	{
		lh->new_node = new_node;
		if (!lh->head)
			lh->head = lh->new_node;
		else
			lh->current->next = lh->new_node;
		lh->current = lh->new_node;
	}
	new_node->sub_type = subtype;
	new_node->op_type = get_op_type(new_node->content, false);
	new_node->io_type = get_io_type(new_node->content);
	return (SUCCESS);
}

bool	is_prev_redirector(const t_list *prev)
{
	if (prev == NULL)
		return (false);
	if (get_io_type(prev->content))
	{
		return (true);
	}
	return (false);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

bool	is_spaces(const char *str, size_t n)
{
	size_t	count;

	count = 0;
	if (!str)
		return (false);
	while (str[count] && (n == 0 || count < n))
	{
		if (!ft_isspace(str[count]))
			return (false);
		count++;
	}
	return (true);
}
