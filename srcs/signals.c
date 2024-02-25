/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:56:23 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/25 06:06:43 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setup_terminal(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


void	ft_signal_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	if (signo == SIGINT){
	    g_sigint_received = 1;
		rl_replace_line("", 0);
        printf("\n");
		}
	else if (signo == SIGUSR1)
		ft_printf("Received SIGUSR1 %d\n", info->si_pid);
	else if (signo == SIGQUIT)
		printf("\n");
	else
		printf("Uknown signal\n");
}

int	init_signals(void)
{
	struct sigaction	action;
	struct sigaction	ignoreaction;

	setup_terminal();

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
