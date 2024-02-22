/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:18:06 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/22 04:18:07 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Displays the history of the shell.
 */
void	ft_displayhistory(void)
{
	HIST_ENTRY	*entry;
	int			i;

	i = 0;
	using_history();
	while (true)
	{
		entry = history_get(history_base + i);
		if (!entry)
			break ;
		printf("%d: %s\n", i + 1, entry->line);
		i++;
	}
}

void	ft_executecmd(t_ShellState *state)
{
	pid_t pid;
	char **cmd_args;
	int pipefd[2];
	int fd_in;

	fd_in = 0;
	for (int i = 0; i < state->cmd_count; i++)
	{
		cmd_args = ft_split(state->parsed_args[i], ' ');
		if (!cmd_args)
			ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
		if (ft_cmdhandler(state, cmd_args) == 1)
			continue ;
		if (i < state->cmd_count - 1)
		{
			if (pipe(pipefd) < 0)
				ft_free_exit(state, ERR_PIPE, EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0)
			ft_free_exit(state, ERR_FORK, EXIT_FAILURE);
		if (pid == 0)
		{
			if (fd_in != 0)
			{
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			if (i < state->cmd_count - 1)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			if (execvp(cmd_args[0], cmd_args) < 0)
			{
				perror("execvp");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (fd_in != 0)
				close(fd_in);
			if (i < state->cmd_count - 1)
			{
				fd_in = pipefd[0];
				close(pipefd[1]);
			}
			wait(NULL);
			for (int j = 0; cmd_args[j] != NULL; j++)
			{
				free(cmd_args[j]);
			}
			free(cmd_args);
		}
	}
}