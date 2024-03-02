/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 19:07:07 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/02 20:13:59 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	env_key_len(const char *env_entry)
{
	size_t	len;

	len = 0;
	if (!env_entry || ft_isdigit(env_entry[0]))
		return (0);
	while (ft_isenv_var(env_entry[len]))
		len++;
	return (len);
}

bool	env_entry_is_valid(const char *entry)
{
	const size_t	len = env_key_len(entry);

	if (len > 0 && entry[len] == '=' && !ft_isdigit(entry[0]))
		return (true);
	return (false);
}

bool	env_name_is_valid(const char *name)
{
	const size_t	len = env_key_len(name);

	if (len > 0 && !ft_isdigit(name[0]) && name[len] == '\0')
		return (true);
	return (false);
}

int	env_get_entry_index(const char *entry, const char **envp)
{
	int	i;
	int	len;

	if (!entry || !envp)
		return (-1);
	i = 0;
	len = env_key_len(entry);
	while (len && envp[i])
	{
		if (ft_strncmp(envp[i], entry, len) == 0)
		{
			if (envp[i][len] == '=')
				return (i);
		}
		i++;
	}
	return (-1);
}
