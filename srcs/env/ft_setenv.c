/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 18:55:46 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/02 21:58:42 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "minishell.h"

static bool	existing_entry_handled(const char *entry, t_shellstate *s)
{
	const int	existing_index
		= env_get_entry_index(entry, (const char **)s->envp);
	char		*new_entry;

	if (existing_index >= 0)
	{
		new_entry = ft_strdup(entry);
		if (new_entry)
		{
			free(s->envp[existing_index]);
			s->envp[existing_index] = new_entry;
		}
		else
			ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
		return (true);
	}
	return (false);
}

int	ft_setenv_entry(const char *entry, t_shellstate *state)
{
	char	**new_envp;

	if (!entry || !state)
		return (0);
	if (!env_entry_is_valid(entry))
		return (-1);
	if (existing_entry_handled(entry, state))
		return (0);
	new_envp = strarrjoin((const char **)state->envp, entry);
	if (new_envp)
	{
		free_str_array(state->envp);
		state->envp = new_envp;
	}
	else
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	return (0);
}

static bool	replaced_existing_entry(char *entry, char **envp)
{
	const int	existing_index
		= env_get_entry_index(entry, (const char **)envp);
	char		*tmp;

	if (existing_index >= 0)
	{
		tmp = envp[existing_index];
		envp[existing_index] = entry;
		free(tmp);
		return (true);
	}
	return (false);
}

static char	*get_new_entry(const char *name, const char *value)
{
	char	*tmp;
	char	*new_entry;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	new_entry = ft_strjoin(tmp, value);
	free(tmp);
	return (new_entry);
}

int	ft_setenv(const char *name, const char *value, t_shellstate *state)
{
	char	*new_entry;
	char	**new_envp;

	if (!name || !state || !value)
		return (0);
	if (!env_name_is_valid(name))
		return (-1);
	new_entry = get_new_entry(name, value);
	if (!new_entry)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	if (!replaced_existing_entry(new_entry, state->envp))
	{
		new_envp = strarrjoin((const char **)state->envp, new_entry);
		free(new_entry);
		if (!new_envp)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		free_str_array(state->envp);
		state->envp = new_envp;
	}
	return (0);
}
