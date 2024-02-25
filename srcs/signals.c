/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:56:23 by apyykone          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/25 06:06:43 by apyykone         ###   ########.fr       */
=======
/*   Updated: 2024/02/25 12:05:17 by ttakala          ###   ########.fr       */
>>>>>>> eaca9d0ab313a9e61b4708c6831f1c71b6189654
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets the terminal to no echo mode.
*/
void setup_terminal(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
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
<<<<<<< HEAD
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		printf("\n");
=======
		printf("\n");
	else if (signo == SIGUSR1)
		printf("Received SIGUSR1 %d\n", info->si_pid);
	else if (signo == SIGQUIT)
		return ;
>>>>>>> eaca9d0ab313a9e61b4708c6831f1c71b6189654
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
