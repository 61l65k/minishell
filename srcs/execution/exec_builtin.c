/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:47:36 by alex              #+#    #+#             */
/*   Updated: 2024/03/07 22:48:32 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_type.h"
#include "builtin.h"
#include "minishell.h"

int		get_command(t_list *cmd, t_command *command);
void	free_command(t_command *command);

/**
 * @brief Handles the built-in commands in the main process.
 */
int	builtin_main(t_shellstate *state, t_list *arg_list)
{
	t_command		command;
	t_builtin_type	builtin_type;
	t_builtin_func	func;

	ft_memset(&command, 0, sizeof(t_command));
	if (get_command(arg_list, &command) == FAILURE)
		ft_free_exit(state, ERR_MALLOC, 1);
	builtin_type = get_builtin_type(command.args[0]);
	if (builtin_type == BI_NOT_BUILTIN)
		return (free_command(&command), BI_NOT_BUILTIN);
	func = get_builtin_func(builtin_type);
	state->last_exit_status = SUCCESS;
	if (func)
	{
		func(command.args, state);
	}
	return (free_command(&command), builtin_type);
}

/**
 * @brief Handles inbuilt commands when forked.
 */
int	builtin_child(t_shellstate *state, t_exechelper *exechelper)
{
	const char				*cmd = exechelper->cmd_arr[0];
	const t_builtin_type	builtin_type = get_builtin_type(cmd);
	const t_builtin_func	func = get_builtin_func(builtin_type);

	if (builtin_type == BI_NOT_BUILTIN)
		return (BI_NOT_BUILTIN);
	state->last_exit_status = SUCCESS;
	if (func)
	{
		func(exechelper->cmd_arr, state);
	}
	exit(state->last_exit_status);
	return (builtin_type);
}

/**
 * @brief Returns the built-in command type.
 * Or 0 if the command is not a built-in.
 */
/**/
t_builtin_type	get_builtin_type(const char *arg)
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
 * @brief Returns whether we are in a pipeline, 
 * and therefore should not run built-ins in main process
 */

bool	is_pipeline(t_shellstate *state, t_exechelper *helper)
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
