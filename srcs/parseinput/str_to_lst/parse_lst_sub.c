/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst_sub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:11:40 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/16 17:11:42 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_trimparentheses(t_lsthelper *lh, bool start)
{
	char	*trimmed_content;

	if (start)
		trimmed_content = ft_strndup(lh->arg + 1, lh->arg_len - 1);
	else
		trimmed_content = ft_strndup(lh->arg, lh->arg_len - 1);
	if (!trimmed_content)
		return (NULL);
	free(lh->arg);
	lh->arg = (char *)trimmed_content;
	return (lh->arg);
}
