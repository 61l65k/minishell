/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:33:49 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/15 22:31:48 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	str_to_long(const char *str, int *overflow)
{
	long	result;
	long	sign;
	long	tmp;

	*overflow = 0;
	if (ft_strcmp(str, "-9223372036854775808") == 0)
		return (LONG_MIN);
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		tmp = result;
		result = result * 10 + (*str++ - '0');
		if (tmp > result && sign == 1)
			return (*overflow = 1, (LONG_MAX));
		else if (tmp > result && sign == -1)
			return (*overflow = -1, (LONG_MIN));
	}
	return (result * sign);
}

static int	is_numeric(const char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i] || !ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(char **args, t_shellstate *state)
{
	long	exit_code;
	int		overflow;

	exit_code = 0;
	if (state->is_child_process == false && isatty(STDIN_FILENO))
		ft_fprintf(STDERR_FILENO, "exit\n");
	if (args[1])
	{
		exit_code = str_to_long(args[1], &overflow);
		if (overflow != 0 || !is_numeric(args[1]))
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", args[1]);
			free(args);
			ft_free_exit(state, NULL, EXIT_INVALID_ARG);
		}
		if (args[2])
		{
			ft_fprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
			state->last_exit_status = 1;
			return ;
		}
	}
	free(args);
	ft_free_exit(state, NULL, exit_code);
}
