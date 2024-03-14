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
# define RESET_E "\001\033[0m\002"
# define DBLUE_E "\001\033[0;94m\002"
# define ERR_GETCWD "\033[0;31mError: getcwd() failed \n\033[0m"
# define ERR_PIPE "\033[0;31mError: pipe() failed\n \033[0m"
# define ERR_FORK "\033[0;31mError: fork() failed\n \033[0m"
# define ERR_QUOTES "\033[0;31mminishell: quotes not closed\n\033[0m"
# define ERR_MALLOC "\033[0;31mError: malloc() failed\n\033[0m"
# define ERR_PARENTHESES "\033[0;31mminishell: parentheses not closed\n\033[0m"
# define ERR_AMBIGUOUS_REDIRECT "minishell: %s: ambiguous redirect\n"
# define HDOC_DELIMMSG \
	"minishell: warning: here-document \
delimited by end-of-file (wanted `%s')\n"
# define ERR_HEREDOC_DELIMITER \
	"minishell: syntax error near unexpected \
token `newline'\n"
# define SIGINT_EXIT 130
# ifdef __APPLE__
#  define EXIT_INVALID_ARG 255
#  define MAC_OS 1
# else
#  define EXIT_INVALID_ARG 2
#  define MAC_OS 0
# endif

typedef enum e_charflag_bits
{
	ESCAPED_BIT,
	ENVVAR_BIT,
	PIPE_BIT,
	AND_BIT,
	OR_BIT,
	QUOTE_BIT,
	REDIR_BIT,
	TILDA_BIT
}		t_charflag_bits;

typedef enum e_operators
{
	OP_NONE,
	OP_AND,
	OP_OR,
	OP_HEREDOC,
	OP_APPEND,
	OP_PIPE,
	OP_REDIRECT_OUT,
	OP_REDIRECT_IN,
	OP_OPERATOR_COUNT
}		t_operators;

enum	e_errors
{
	NO_ERROR,
	FALSE_INPUT,
	QUOTE_ERROR = 1,
	SYNTAX_ERROR = 2
};

#endif
