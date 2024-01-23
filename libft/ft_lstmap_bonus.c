/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:16:11 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/26 11:54:15 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*firstnode(void (*del)(void *), void *content)
{
	t_list	*firstnode;

	firstnode = NULL;
	{
		firstnode = ft_lstnew(content);
		if (!firstnode)
		{
			ft_lstclear(&firstnode, del);
			return (NULL);
		}
	}
	return (firstnode);
}

static int	othernodes(t_list **new, void (*del)(void *), void *content)
{
	t_list	*temp;
	t_list	*newlist;

	newlist = *new;
	temp = ft_lstnew(content);
	if (!temp)
	{
		ft_lstclear(&newlist, del);
		return (-1);
	}
	ft_lstadd_back(&newlist, temp);
	return (0);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*current;
	void	*content;

	newlist = NULL;
	current = lst;
	while (current)
	{
		content = (*f)(current->content);
		if (content && newlist == NULL)
		{
			newlist = firstnode(del, content);
			if (!newlist)
				return (NULL);
		}
		else if (content)
		{
			if (othernodes(&newlist, del, content) == -1)
				return (NULL);
		}
		current = current->next;
	}
	return (newlist);
}
