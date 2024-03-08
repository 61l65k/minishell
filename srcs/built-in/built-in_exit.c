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
#include <unistd.h>

static int	is_numeric(const char *str)
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
			ft_fprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", args[1]);
			free(args);
			ft_free_exit(state, NULL, 2);
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			state->last_exit_status = 1;
			return ;
		}
		exit_code = ft_atoi(args[1]);
	}
	free(args);
	ft_free_exit(state, NULL, exit_code);
}
