/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:47:36 by alex              #+#    #+#             */
/*   Updated: 2024/03/09 00:09:02 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles built-in commands in the main process.
 * Parses redirects and argv from arg_list.
 * and stores them to a local t_command struct.
 * Sets up possible redirections and runs the built-in command.
 * @return Returns 0 if the cmd found in arg_list is not a built-in.
 */
int	builtin_main(t_shellstate *state, t_list *arg_list)
{
	t_command		command;
	t_builtin_type	builtin_type;
	t_builtin_func	func;

	command = (t_command){0};
	if (get_command(arg_list, &command) == FAILURE)
		ft_free_exit(state, ERR_MALLOC, 1);
	builtin_type = get_builtin_type(command.argv[0]);
	if (builtin_type == BI_NOT_BUILTIN)
		return (free_command(&command), BI_NOT_BUILTIN);
	func = get_builtin_func(builtin_type);
	state->last_exit_status = SUCCESS;
	if (command.io_vec.len > 0)
	{
		state->last_exit_status = apply_main_process_redirections(&command);
	}
	if (func && state->last_exit_status == SUCCESS)
		func(command.argv, state);
	if (command.io_vec.len > 0)
		restore_main_process_fds(&command);
	return (free_command(&command), builtin_type);
}

/**
 * @brief Handles inbuilt commands in child process.
 * Returns 0 if the cmd is not a built-in,
 * otherwise runs the built-in command and exits the child process.
 */
int	builtin_child(t_shellstate *state, t_exechelper *exechelper)
{
	const char				*cmd = exechelper->cmd_argv[0];
	const t_builtin_type	builtin_type = get_builtin_type(cmd);
	const t_builtin_func	func = get_builtin_func(builtin_type);

	if (builtin_type == BI_NOT_BUILTIN)
		return (BI_NOT_BUILTIN);
	state->last_exit_status = SUCCESS;
	if (func)
	{
		func(exechelper->cmd_argv, state);
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
 * @brief Returns true if current command is part of a pipeline.
 * If in pipeline, we need to fork even if the command is a built-in.
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
