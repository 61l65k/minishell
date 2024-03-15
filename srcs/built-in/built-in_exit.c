/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:33:49 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/09 00:02:48 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	str_to_long(const char *str)
{
	long	result;
	long	sign;
	long	tmp;

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
			return (LONG_MAX);
		else if (tmp > result && sign == -1)
			return (LONG_MIN);
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

	exit_code = 0;
	if (state->is_child_process == false)
		ft_fprintf(FD_EXIT_MSG, "exit\n");
	if (args[1])
	{
		exit_code = str_to_long(args[1]);
		if (!is_numeric(args[1]) || exit_code < INT_MIN || exit_code > INT_MAX)
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", args[1]);
			free(args);
			ft_free_exit(state, NULL, EXIT_INVALID_ARG);
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			state->last_exit_status = 1;
			return ;
		}
	}
	free(args);
	ft_free_exit(state, NULL, exit_code);
}
