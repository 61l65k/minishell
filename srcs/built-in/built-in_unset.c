/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:31:35 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/28 19:28:23 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

static void	remove_str_from_str_arr(char *str_to_remove, t_shellstate *s)
{
	const size_t	len = ft_strlen(str_to_remove);
	int				i;
	char			*tmp;

	i = 0;
	while (len && s->envp && s->envp[i])
	{
		if (ft_strncmp(s->envp[i], str_to_remove, len) == 0)
		{
			tmp = s->envp[i];
			while (s->envp[i])
			{
				s->envp[i] = s->envp[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
	if (s->envp_malloced)
		free(tmp);
}

// TODO needs to be reimplemented where we are working with __environ
// that is malloced by us and not the system so we can:
// implement export that adds strings to _environ which can be safely freed
void	builtin_unset(char **args, t_shellstate *state)
{
	char	*ptr_to_value;
	int		i;

	if (!args || !args[0] || !args[1])
		return ;
	i = 0;
	while (args[++i])
	{
		ptr_to_value = getenv(args[i]);
		if (ptr_to_value)
			remove_str_from_str_arr(args[i], state);
	}
}
