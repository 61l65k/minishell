/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_echo_pwd_env.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:00:27 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/06 12:47:41 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(char **args, t_shellstate *state)
{
	const int	n_option = (args[1] != NULL && ft_strcmp(args[1], "-n") == 0);
	size_t		i;

	(void)state;
	i = 0 + n_option;
	while (args[++i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (!n_option)
		ft_putchar_fd('\n', 1);
}

void	builtin_pwd(char **args, t_shellstate *state)
{
	char	cwd[PATH_MAX];

	(void)args;
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	}
	ft_putendl_fd(cwd, 1);
}

void	builtin_env(char **args, t_shellstate *state)
{
	size_t	i;

	(void)args;
	i = 0;
	while (state->envp[i])
	{
		ft_putendl_fd(state->envp[i], 1);
		i++;
	}
}
