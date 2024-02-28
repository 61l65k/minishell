/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:31:35 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/28 21:40:24 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

static void	remove_env_var(char *var_key, t_shellstate *s)
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
			if (s->envp_malloced)
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

	if (!args || !args[0] || !args[1])
		return ;
	i = 0;
	while (args[++i])
	{
		ptr_to_value = getenv(args[i]);
		if (ptr_to_value)
			remove_env_var(args[i], state);
	}
}
