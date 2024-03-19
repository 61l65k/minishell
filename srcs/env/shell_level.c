/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 21:53:01 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/19 22:23:08 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_shell_level(char *const envp[])
{
	const char	*shlvl_str = ft_getenv("SHLVL", envp);
	int			shell_level;
	int			overflow;

	if (shlvl_str == NULL)
		return (-1);
	if (!is_numeric(shlvl_str))
		return (-1);
	shell_level = str_to_long(shlvl_str, &overflow);
	if (overflow || shell_level < 0)
		return (-1);
	return (shell_level);
}

void	update_shell_level(t_shellstate *state)
{
	const int	prev_shlvl = get_shell_level(state->envp);
	int			new_shlvl;
	char		*new_shlvl_str;

	if (prev_shlvl < 0)
		new_shlvl = 1;
	else
		new_shlvl = prev_shlvl + 1;
	new_shlvl_str = ft_itoa(new_shlvl);
	if (new_shlvl_str == NULL)
		ft_free_exit(state, ERR_MALLOC, 1);
	ft_setenv("SHLVL", new_shlvl_str, state);
	free(new_shlvl_str);
}
