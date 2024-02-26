/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:54:51 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 12:20:02 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAXCOM 1000
# define MAXLIST 100
# define MAX_PROMPT 1060
# define SUCCESS 0
# define MAX_VAR_LENGTH 100
# include "libft.h"
# include "minimessages.h"
# include "miniutils.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_shellstate
{
	char		input_string[MAXCOM];
	char		**parsed_args;
	bool		is_piped;
	int			cmd_count;
	t_redirs	*commands;
}				t_shellstate;

void			ft_free_exit(t_shellstate *state, const char *errormsg,
					int exitcode);
int				ft_cmdhandler(t_shellstate *state, char **parsed);
void			ft_executecmd(t_shellstate *state);
void			ft_free_resets(t_shellstate *state);
void			ft_displayhistory(void);
int				init_signals(void);
void			setup_terminal(void);
void			execute_cmd(char *file, char **cmd_argv);

#endif