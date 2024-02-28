/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:31:35 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/28 13:43:28 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_str_from_str_arr(char **env_arr, char *str_to_remove)
{
	const size_t	len = ft_strlen(str_to_remove);
	int				i;

	i = 0;
	while (len && env_arr && env_arr[i])
	{
		if (ft_strncmp(env_arr[i], str_to_remove, len) == 0)
		{
			while (env_arr[i])
			{
				env_arr[i] = env_arr[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
}

// TODO needs to be reimplemented where we are working with __environ
// that is malloced by us and not the system so we can:
// implement export that adds strings to _environ which can be safely freed
void	builtin_unset(char **args)
{
	char	*ptr_to_value;
	int		i;
char ** env = NULL;
	if (!args || !args[0] || !args[1])
		return ;
	i = 0;
	while (args[++i])
	{
		ptr_to_value = getenv(args[i]);
		if (ptr_to_value)
			remove_str_from_str_arr(env, args[i]);
	}
}
