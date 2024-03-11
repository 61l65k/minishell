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
# define CYAN_E "\001\033[0;36m\002"
# define RESET_E "\001\033[0m\002"
# define RED_E "\001\033[0;31m\002"
# define GREEN_E "\001\033[0;32m\002"
# define BLUE_E "\001\033[0;34m\002"
# define DBLUE_E "\001\033[0;94m\002"
# define ERR_READLINE "\033[0;31mError: readline() failed \n\033[0m"
# define ERR_PROCESTRING "\033[0;31mError: processing string\n\033[0m"
# define ERR_EXECV "\033[0;31mError: execv() failed\n\033[0m"
# define ERR_GETCWD "\033[0;31mError: getcwd() failed \n\033[0m"
# define ERR_PIPE "\033[0;31mError: pipe() failed\n \033[0m"
# define ERR_FORK "\033[0;31mError: fork() failed\n \033[0m"
# define ERR_SIGACTION "\033[0;31mError: sigaction() failed\n\033[0m"
# define ERR_QUOTES "\033[0;31mminishell: quotes not closed\n\033[0m"
# define ERR_MALLOC "\033[0;31mError: malloc() failed\n\033[0m"
# define ERR_PARENTHESES "\033[0;31mminishell: parentheses not closed\n\033[0m"

# define HDOC_DELIMMSG \
	"minishell: warning: here-document \
delimited by end-of-file (wanted `%s')\n"
# define ERR_HEREDOC_DELIMITER \
	"minishell: syntax error near unexpected \
token `newline'\n"
# define SIGINT_EXIT 130

enum	e_errors
{
	NO_ERROR,
	FALSE_INPUT,
	QUOTE_ERROR = 1,
};

#endif
