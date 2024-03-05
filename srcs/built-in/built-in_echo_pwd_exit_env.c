/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_echo_pwd_exit_env.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:00:27 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/05 21:06:50 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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

int	is_numeric(const char *str)
{
	if (str[0] == '-' || str[0] == '+')
		str++;
	if (!*str || !ft_isdigit(*str))
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	builtin_exit(char **args, t_shellstate *state)
{
	int	exit_code;

	exit_code = 0;
	if (state->is_child_process == false)
		printf("exit\n");
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_fprintf(2, "exit: %s: numeric argument required\n", args[1]);
			free(args);
			ft_free_exit(state, NULL, 2);
		}
		if (args[2])
		{
			free(args);
			ft_putendl_fd("exit: too many arguments", 2);
			ft_free_exit(state, NULL, 1);
		}
		exit_code = ft_atoi(args[1]);
	}
	free(args);
	ft_free_exit(state, NULL, exit_code);
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
