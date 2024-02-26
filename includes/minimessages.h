/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimessages.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:54:43 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/22 04:54:44 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMESSAGES_H
# define MINIMESSAGES_H
# define CLEAR_SCREEN "\033[H\033[J"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define ERR_PROCESTRING "\033[0;31mError: processing string\n\033[0m"
# define ERR_EXECV "\033[0;31mError: execv() failed\n\033[0m"
# define ERR_GETCWD "\033[0;31mError: getcwd() failed \n\033[0m"
# define ERR_PIPE "\033[0;31mError: pipe() failed\n \033[0m"
# define ERR_FORK "\033[0;31mError: fork() failed\n \033[0m"
# define ERR_SIGACTION "\033[0;31mError: sigaction() failed\n\033[0m"
# define ERR_QUOTES "\033[0;31mError: quotes not closed\n\033[0m"
# define HELLO_MSG \
	"\nHello %s.\nMind that this is \
not a place to play around.\
\nUse help to know more..\n"
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
