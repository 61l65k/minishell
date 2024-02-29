/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:56:23 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/28 20:33:07 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

/**
 * @brief Sets the terminal to no echo mode.
 * & Executes the .minishellrc file.
 */
void	setup_terminal(void)
{
	int				fd;
	char			*line;
	struct termios	term;

	fd = open(".minishellrc", O_RDONLY);
	if (fd != -1)
	{
		while (true)
		{
			line = get_next_line(fd);
			if (!line)
				break ;
			system(line);
			free(line);
		}
		close(fd);
	}
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	term.c_lflag |= (ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * @brief Callback handler for signals received
 */
static void	ft_signal_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		write(1, "\n", 1);
	else
		printf("Unknown signal\n");
}

/**
 * @brief Initializes all signal handlers & their callbacks.
 */
int	init_signals(void)
{
	struct sigaction	action;
	struct sigaction	ignoreaction;

	sigemptyset(&action.sa_mask);
	sigemptyset(&ignoreaction.sa_mask);
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = ft_signal_handler;
	ignoreaction.sa_handler = SIG_IGN;
	ignoreaction.sa_flags = 0;
	if (sigaction(SIGUSR1, &action, NULL) == -1 || sigaction(SIGINT, &action,
			NULL) == -1 || sigaction(SIGQUIT, &ignoreaction, NULL) == -1)
	{
		perror("Error: sigaction() failed");
		exit(EXIT_FAILURE);
	}
	return (0);
}
