/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:47:45 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/29 14:26:10 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_declare(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		ft_printf("declare -x %s\n", envp[i]);
		i++;
	}
}

static int	get_env_entry_index(char *entry, char **envp)
{
	int	i;
	int	len;

	if (!entry || !envp)
		return (-1);
	i = 0;
	len = ft_envlen(entry);
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

static bool	existing_entry_handled(char **args, int i, t_shellstate *s)
{
	const int	existing_entry_index = get_env_entry_index(args[i], s->envp);
	char		*new_entry;

	if (existing_entry_index >= 0)
	{
		new_entry = ft_strdup(args[i]);
		if (new_entry)
		{
			free(s->envp[existing_entry_index]);
			s->envp[existing_entry_index] = new_entry;
		}
		else
			ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
		return (true);
	}
	return (false);
}

static bool	entry_is_valid(char *entry)
{
	const size_t	len = ft_envlen(entry);

	if (len > 0 && entry[len] == '=')
		return (true);
	ft_fprintf(2, "export: `%s': not a valid identifier\n", entry);
	return (false);
}

void	builtin_export(char **args, t_shellstate *state)
{
	int		i;
	char	**new_envp;

	if (!args || !args[0])
		return ;
	if (!args[1])
		return (print_env_declare(state->envp));
	i = 0;
	while (args[++i])
	{
		if (!entry_is_valid(args[i]))
			continue ;
		if (existing_entry_handled(args, i, state))
			continue ;
		new_envp = strarrjoin((const char **)state->envp, args[i]);
		if (new_envp)
		{
			free_str_array(state->envp);
			state->envp = new_envp;
		}
		else
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	}
}
