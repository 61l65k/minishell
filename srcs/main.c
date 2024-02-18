/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/18 21:50:06 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define GREEN "\033[0;33m"
#define MAX_PROMPT 1060

void	init_shell(void)
{
	char	*username;

	clear();


	username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
	sleep(1);
	clear();
}

// Function to take input
int	takeInput(char *str)
{
	char	cwd[1024];
	char	prompt[MAX_PROMPT] = GREEN "➜ " RESET CYAN;
	char	*buf;

	// Get current working directory
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return (1);
	}
	// Concatenate cwd and reset color to the prompt
	strncat(prompt, cwd, MAX_PROMPT - strlen(prompt) - strlen(" " RESET) - 1);
	strcat(prompt, " " RESET);
	// Use readline with the constructed prompt
	buf = readline(prompt);
	if (buf && strlen(buf) != 0)
	{
		add_history(buf);
		strcpy(str, buf);
		free(buf);
		return (0);
	}
	else
	{
		if (buf)
			free(buf);
		return (1);
	}
}

// Function where the system command is executed
void	execArgs(char **parsed)
{
	pid_t	pid;

	// Forking a child
	pid = fork();
	if (pid == -1)
	{
		printf("\nFailed forking child..");
		return ;
	}
	else if (pid == 0)
	{
		if (execvp(parsed[0], parsed) < 0)
			printf("\nCould not execute command..");
		exit(0);
	}
	else
	{
		// waiting for child to terminate
		wait(NULL);
		return ;
	}
}

// Function where the piped system commands is executed
void	execArgsPiped(char **parsed, char **parsedpipe)
{
	int	pipefd[2];

	// 0 is read end, 1 is write end
	pid_t p1, p2;
	if (pipe(pipefd) < 0)
	{
		printf("\nPipe could not be initialized");
		return ;
	}
	p1 = fork();
	if (p1 < 0)
	{
		printf("\nCould not fork");
		return ;
	}
	if (p1 == 0)
	{
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (execvp(parsed[0], parsed) < 0)
		{
			printf("\nCould not execute command 1..");
			exit(0);
		}
	}
	else
	{
		// Parent executing
		p2 = fork();
		if (p2 < 0)
		{
			printf("\nCould not fork");
			return ;
		}
		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0)
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0)
			{
				printf("\nCould not execute command 2..");
				exit(0);
			}
		}
		else
		{
			wait(NULL);
			wait(NULL);
		}
	}
}

void	openHelp(void)
{
	puts("\n***WELCOME TO MY SHELL HELP***"
			"\nCopyright @ Suprotik Dey"
			"\n-Use the shell at your own risk..."
			"\nList of Commands supported:"
			"\n>cd"
			"\n>ls"
			"\n>exit"
			"\n>all other general commands available in UNIX shell"
			"\n>pipe handling"
			"\n>improper space handling");
	return ;
}

void displayHistory(void)
{
    HIST_ENTRY *entry;
    int i = 0;

    using_history();
    
    while ((entry = history_get(history_base + i)) != NULL) {
        printf("%d: %s\n", i + 1, entry->line);
        i++;
    }
}


int	ownCmdHandler(char **parsed)
{
	char	*ListOfOwnCmds[5] = {"exit", "cd", "help", "hello", "history"};
	char	*username;
	int		switchOwnArg;
	int		i;

	switchOwnArg = 0;
	for (i = 0; i < 5; i++)
	{
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0)
		{
			switchOwnArg = i + 1;
			break ;
		}
	}
	switch (switchOwnArg)
	{
	case 1:
		printf("\nGoodbye\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return (1);
	case 3:
		openHelp();
		return (1);
	case 4:
		username = getenv("USER");
		printf("\nHello %s.\nMind that this is "
				"not a place to play around."
				"\nUse help to know more..\n",
				username);
		return (1);
	case 5:
		displayHistory();
		return (1);
	default:
		break ;
	}
	return (0);
}

// function for finding pipe
int	parsePipe(char *str, char **strpiped)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i++] == NULL)
			break ;
	}
	if (strpiped[1] == NULL)
		return (0); // returns zero if no pipe is found.
	else
	{
		return (1);
	}
}

// function for parsing command words
void	parseSpace(char *str, char **parsed)
{
	int	i;

	for (i = 0; i < MAXLIST; i++)
	{
		parsed[i] = strsep(&str, " ");
		if (parsed[i] == NULL)
			break ;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int	processString(char *str, char **parsed, char **parsedpipe)
{
	char	*strpiped[2];
	int		piped;

	piped = 0;
	piped = parsePipe(str, strpiped);
	if (piped)
	{
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);
	}
	else
		parseSpace(str, parsed);
	if (ownCmdHandler(parsed))
		return (0);
	else
		return (1 + piped);
}

int	main(void)
{
	char	inputString[MAXCOM], *parsedArgs[MAXLIST];
	char	*parsedArgsPiped[MAXLIST];
	int		execFlag;

	execFlag = 0;
	init_shell();
	while (1)
	{
		// take input
		if (takeInput(inputString))
			continue ;
		// process
		execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.
		// execute
		if (execFlag == 1)
			execArgs(parsedArgs);
		if (execFlag == 2)
			execArgsPiped(parsedArgs, parsedArgsPiped);
	}
	return (0);
}
