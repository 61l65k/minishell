/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:55:29 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/06 11:19:13 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == -1)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static char	*get_full_path(char **env_paths_arr, const char *file)
{
	char	*result;
	char	*file_with_slash;

	file_with_slash = ft_strjoin("/", file);
	if (!file_with_slash)
		return (NULL);
	result = NULL;
	while (*env_paths_arr)
	{
		result = ft_strjoin(*env_paths_arr, file_with_slash);
		if (access(result, X_OK) == 0)
		{
			free(file_with_slash);
			return (result);
		}
		free(result);
		env_paths_arr++;
	}
	free(file_with_slash);
	return (NULL);
}

static char	*get_path_to_file(const char *file, const char *env_path)
{
	char	**str_arr_paths;
	char	*full_path;

	if (!file || !*file)
		return (NULL);
	if (!env_path || file[0] == '/' || file[0] == '.')
		return (ft_strdup(file));
	str_arr_paths = ft_split(env_path, ':');
	if (!str_arr_paths)
		return (NULL);
	if (str_arr_paths[0])
	{
		full_path = get_full_path(str_arr_paths, file);
		if (!full_path)
			full_path = ft_strdup(file);
		free_str_array(str_arr_paths);
		return (full_path);
	}
	free_str_array(str_arr_paths);
	return (ft_strdup(file));
}

static void	exec_error_exit(char *cmd_path, char *error_msg, int exit_code)
{
	if (cmd_path)
		ft_fprintf(2, "%s: %s\n", cmd_path, error_msg);
	else
		ft_fprintf(2, "%s\n", error_msg);
	free(cmd_path);
	exit(exit_code);
}

/**
 * @brief Wrapper for execve(). Searches for file in PATH and executes it.
 * Either exits with an error or, on succesful execve, replaces the
 * current process, never returning to the caller.
 */
void	ft_execvp(const char *file, char *const argv[], char *const envp[])
{
	const char	*env_path = ft_getenv("PATH", envp);
	char		*cmd_path;

	cmd_path = get_path_to_file(file, env_path);
	(void)file;
	(void)env_path;
	if (!cmd_path)
		exec_error_exit(NULL, "malloc failed while parsing path", 1);
	else if (ft_strchr(cmd_path, '/') == NULL)
		exec_error_exit(cmd_path, "command not found", 127);
	else if (is_directory(cmd_path))
		exec_error_exit(cmd_path, "Is a directory", 126);
	else if (access(cmd_path, F_OK) != 0)
		exec_error_exit(cmd_path, "No such file or directory", 127);
	else if (access(cmd_path, X_OK) != 0)
		exec_error_exit(cmd_path, "Permission denied", 126);
	else if (execve(cmd_path, argv, envp) == -1)
		exec_error_exit(cmd_path, strerror(errno), errno);
}
