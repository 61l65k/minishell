/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:31:35 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/03 00:53:15 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(char *var_key, t_shellstate *s)
{
	const size_t	len = ft_strlen(var_key);
	int				i;
	char			*tmp;

	tmp = NULL;
	i = 0;
	while (len && s->envp && s->envp[i])
	{
		if (!ft_strncmp(s->envp[i], var_key, len) && s->envp[i][len] == '=')
		{
			tmp = s->envp[i];
			while (s->envp[i])
			{
				s->envp[i] = s->envp[i + 1];
				i++;
			}
			free(tmp);
			break ;
		}
		i++;
	}
}

void	builtin_unset(char **args, t_shellstate *state)
{
	char	*ptr_to_value;
	int		i;

	state->last_exit_status = 0;
	if (!args || !args[0] || !args[1])
		return ;
	i = 0;
	while (args[++i])
	{
		if (MAC_OS && !env_name_is_valid(args[i]))
		{
			state->last_exit_status = 1;
			ft_fprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", args[i]);
			continue ;
		}
		ptr_to_value = ft_getenv(args[i], state->envp);
		if (ptr_to_value)
			remove_env_var(args[i], state);
	}
}
