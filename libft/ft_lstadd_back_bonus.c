/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 23:06:40 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/26 14:48:54 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*current;

	if (lst && new_node)
	{
		if (*lst == NULL)
			*lst = new_node;
		else
		{
			current = *lst;
			while (current->next)
				current = current->next;
			current->next = new_node;
		}
	}
}
