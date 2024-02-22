#ifndef MINIERROR_H
# define MINIERROR_H
# define CLEAR "\033[H\033[J"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define ERR_PROCESTRING "\033[0;31mError: processing string\n \033[0m"
# define ERR_EXECV "\033[0;31mError: execv() failed\n \033[0m"
# define ERR_GETCWD "\033[0;31mError: getcwd() failed \n \033[0m"
# define ERR_PIPE "\033[0;31mError: pipe() failed\n \033[0m"
# define ERR_FORK "\033[0;31mError: fork() failed\n \033[0m"
# define HELP_MSG \
	"\n***WELCOME TO MINISHELL HELP***\
\n-Use the shell at your own risk...\
\nList of Commands supported:\
\n$ cd\
\n$ ls\
\n$ exit\
\n$ all other general commands available in UNIX shell\
\n$ pipe handling\
\n$ improper space handling\n"

enum	e_errors
{
	NO_ERROR,
	FALSE_INPUT,
};

#endif