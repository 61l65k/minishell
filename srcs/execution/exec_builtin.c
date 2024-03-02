/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:47:36 by alex              #+#    #+#             */
/*   Updated: 2024/03/02 22:36:50 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the built-in commands.
 * & Returns 1 if the command is handled, otherwise 0.
 */
int	ft_builtin_cmdhandler(t_shellstate *state, char **parsed)
{
	if (!parsed || !parsed[0])
		return (FOUNDCMD);
	if (ft_strcmp(parsed[0], "exit") == 0)
		ft_free_exit(state, NULL, EXIT_SUCCESS);
	else if (ft_strcmp(parsed[0], "cd") == 0)
		return (builtin_cd(parsed[1], state), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "help") == 0)
		return (printf(HELP_MSG), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "hello") == 0)
		return (printf(HELLO_MSG, ft_getenv("USER", state->envp)), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "unset") == 0)
		return (builtin_unset(parsed, state), FOUNDCMD);
	else if (ft_strcmp(parsed[0], "export") == 0)
		return (builtin_export(parsed, state), FOUNDCMD);
	return (SUCCESS);
}
