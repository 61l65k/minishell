/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:47:45 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/03 00:33:59 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "minishell.h"

static void	print_env_declare(const char **envp)
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

void	builtin_export(char **args, t_shellstate *state)
{
	int		i;

	state->last_exit_status = 0;
	if (!args || !args[0])
		return ;
	if (!args[1])
		return (print_env_declare((const char **)state->envp));
	i = 0;
	while (args[++i])
	{
		if (ft_setenv_entry(args[i], state) == -1)
		{
			state->last_exit_status = 1;
			ft_fprintf(2, "export: `%s': not a valid identifier\n", args[i]);
		}
	}
}
