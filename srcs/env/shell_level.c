/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 21:53:01 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/19 23:24:23 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_shell_level(char *const envp[])
{
	const char	*shlvl_str = ft_getenv("SHLVL", envp);
	int			shell_level;

	if (shlvl_str == NULL)
		return (0);
	if (!is_numeric(shlvl_str))
		return (0);
	shell_level = ft_atoi(shlvl_str);
	return (shell_level);
}

void	update_shell_level(t_shellstate *state)
{
	const int	prev_shlvl = get_shell_level(state->envp);
	int			new_shlvl;
	char		*new_shlvl_str;

	new_shlvl = prev_shlvl + 1;
	if (new_shlvl > 999)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: warning: shell level (%d) too high, resetting to 1\n",
			new_shlvl);
		new_shlvl = 1;
	}
	else if (new_shlvl < 0)
		new_shlvl = 0;
	new_shlvl_str = ft_itoa(new_shlvl);
	if (new_shlvl_str == NULL)
		ft_free_exit(state, ERR_MALLOC, 1);
	ft_setenv("SHLVL", new_shlvl_str, state);
	free(new_shlvl_str);
}
