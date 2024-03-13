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

volatile sig_atomic_t	g_signal_flag = 0;

void	check_g_signal_flag(t_shellstate *s)
{
	if (g_signal_flag)
	{
		s->last_exit_status = g_signal_flag;
		g_signal_flag = 0;
	}
}

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
void	ft_signal_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signo == SIGINT)
	{
		g_signal_flag = 130;
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
	if (sigaction(SIGINT, &action, NULL) < 0 || sigaction(SIGQUIT,
			&ignoreaction, NULL) < 0)
	{
		perror("Error: sigaction() failed");
		exit(EXIT_FAILURE);
	}
	return (0);
}
