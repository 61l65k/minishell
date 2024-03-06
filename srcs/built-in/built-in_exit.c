/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:33:49 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/06 12:47:44 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ft_fprintf(2, "exit: %s: numeric argument required\n", args[1]);
			free(args);
			ft_free_exit(state, NULL, 2);
		}
		if (args[2])
		{
			ft_putendl_fd("exit: too many arguments", 2);
			state->last_exit_status = 1;
			return ;
		}
		exit_code = ft_atoi(args[1]);
	}
	free(args);
	ft_free_exit(state, NULL, exit_code);
}
