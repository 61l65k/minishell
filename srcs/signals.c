/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:56:23 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/25 12:05:17 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	if (signo == SIGINT)
		printf("\n");
	else if (signo == SIGUSR1)
		printf("Received SIGUSR1 %d\n", info->si_pid);
	else if (signo == SIGQUIT)
		return ;
	else
		printf("Unknown signal\n");
}

int	init_signals(void)
{
	pid_t				pid;
	struct sigaction	action;
	sigset_t			block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGQUIT);
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = ft_signal_handler;
	if (sigaction(SIGUSR1, &action, NULL) == -1 || sigaction(SIGINT, &action,
			NULL) == -1)
	{
		printf("Error setting up signal handler\n");
		exit(EXIT_FAILURE);
	}
	action.sa_flags = 0;
	action.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &action, NULL) == -1)
	{
		printf("Error blocking SIGQUIT\n");
		exit(EXIT_FAILURE);
	}
	pid = getpid();
	printf("PID = %d\n", pid);
	return (0);
}
