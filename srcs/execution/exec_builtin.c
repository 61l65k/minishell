/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:47:36 by alex              #+#    #+#             */
/*   Updated: 2024/03/04 16:56:07 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

/**
 * @brief Handles the built-in commands.
 * & Returns 0 if the command was not handled as built-in.
 * If child_process is true, and the command is a built-in, the function will
 * exit with the built-in's exit status.
 */
int	ft_builtin_cmdhandler(
	t_shellstate *state,
	t_exechelper *h,
	bool child_process)
{
	const t_builtin_type	is_builtin = get_builtin_type(h->cmd_args[0]);
	const t_builtin_func	func = get_builtin_func(is_builtin);
	const bool				should_fork = builtin_should_fork(state, h);

	if (is_builtin == BI_NOT_BUILTIN)
		return (BI_NOT_BUILTIN);
	if (should_fork && !child_process)
		return (BI_NOT_BUILTIN);
	state->last_exit_status = SUCCESS;
	if (func)
		func(h->cmd_args, state);
	if (child_process)
		exit(state->last_exit_status);
	return (is_builtin);
}

/**
 * @brief Returns the built-in command type.
 * Or 0 if the command is not a built-in.
 */

t_builtin_type	get_builtin_type(char *arg)
{
	if (!arg)
		return (BI_EMPTY);
	else if (ft_strcmp(arg, "echo") == 0)
		return (BI_ECHO);
	else if (ft_strcmp(arg, "cd") == 0)
		return (BI_CD);
	else if (ft_strcmp(arg, "pwd") == 0)
		return (BI_PWD);
	else if (ft_strcmp(arg, "export") == 0)
		return (BI_EXPORT);
	else if (ft_strcmp(arg, "unset") == 0)
		return (BI_UNSET);
	else if (ft_strcmp(arg, "env") == 0)
		return (BI_ENV);
	else if (ft_strcmp(arg, "exit") == 0)
		return (BI_EXIT);
	return (BI_NOT_BUILTIN);
}

/**
 * @brief Returns the built-in command function.
 * Or NULL in case of not built-in or empty name.
 */
t_builtin_func	get_builtin_func(t_builtin_type type)
{
	static const t_builtin_func	funcs[] = {
	[BI_NOT_BUILTIN] = NULL,
	[BI_EMPTY] = NULL,
	[BI_ECHO] = &builtin_echo,
	[BI_CD] = &builtin_cd,
	[BI_PWD] = &builtin_pwd,
	[BI_EXPORT] = &builtin_export,
	[BI_UNSET] = &builtin_unset,
	[BI_ENV] = &builtin_env,
	[BI_EXIT] = &builtin_exit};

	return (funcs[type]);
}

/**
 * @brief Returns whether built-in should be forked or not.
 */

bool	builtin_should_fork(t_shellstate *state, t_exechelper *helper)
{
	int	i;

	i = 0;
	if (state->operators)
	{
		while (state->operators[i] && i <= helper->i)
		{
			if (state->operators[i] == OP_PIPE)
				return (true);
			i++;
		}
	}
	return (false);
}
