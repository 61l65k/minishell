/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:32:17 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/03 00:54:25 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "minishell.h"

static void	handle_home(t_shellstate *state, char *cwd)
{
	const char	*home_path = ft_getenv("HOME", state->envp);

	if (home_path == NULL)
	{
		state->last_exit_status = 1;
		ft_putstr_fd("cd: HOME not set\n", 2);
		return ;
	}
	else if (chdir(home_path) == -1)
	{
		state->last_exit_status = errno;
		ft_fprintf(2, "cd: %s: %s\n", home_path, strerror(errno));
		return ;
	}
	else
	{
		state->last_exit_status = SUCCESS;
		ft_setenv("OLDPWD", cwd, state);
		if (getcwd (cwd, PATH_MAX) == NULL)
			ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
		ft_setenv("PWD", cwd, state);
	}
}

static void	handle_previous(t_shellstate *state, char *cwd)
{
	const char	*oldpwd = ft_getenv("OLDPWD", state->envp);

	if (oldpwd == NULL)
	{
		state->last_exit_status = 1;
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return ;
	}
	else if (chdir(oldpwd) == -1)
	{
		state->last_exit_status = errno;
		ft_fprintf(2, "cd: %s: %s\n", oldpwd, strerror(errno));
		return ;
	}
	else
	{
		state->last_exit_status = SUCCESS;
		ft_setenv("OLDPWD", cwd, state);
		if (getcwd (cwd, PATH_MAX) == NULL)
			ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
		ft_setenv("PWD", cwd, state);
		printf("%s\n", cwd);
	}
}

static void	handle_normal_path(const char *path, t_shellstate *state, char *cwd)
{
	if (chdir(path) == -1)
	{
		state->last_exit_status = errno;
		ft_fprintf(2, "cd: %s: %s\n", path, strerror(errno));
		return ;
	}
	else
	{
		state->last_exit_status = SUCCESS;
		ft_setenv("OLDPWD", cwd, state);
		if (getcwd (cwd, PATH_MAX) == NULL)
			ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
		ft_setenv("PWD", cwd, state);
	}
}

void	builtin_cd(const char *path, t_shellstate *state)
{
	static char	cwd[PATH_MAX];

	state->last_exit_status = 0;
	if (getcwd(cwd, PATH_MAX) == NULL)
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	if (path == NULL)
	{
		handle_home(state, cwd);
		return ;
	}
	else if (ft_strncmp(path, "-", 2) == 0)
	{
		handle_previous(state, cwd);
		return ;
	}
	else
	{
		handle_normal_path(path, state, cwd);
		return ;
	}
}
