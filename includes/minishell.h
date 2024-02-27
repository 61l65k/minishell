/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:54:51 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 20:42:19 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAXCOM 1000
# define MAX_PROMPT 1060
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
	int			last_exit_status;
	t_redirs	*commands;
}				t_shellstate;

void			ft_free_exit(t_shellstate *state, const char *errormsg,
					int exitcode);
int				ft_cmdhandler(t_shellstate *state, char **parsed);
int				ft_executecmd(t_shellstate *state);
void			ft_free_resets(t_shellstate *state);
void			ft_displayhistory(void);
int				init_signals(void);
void			setup_terminal(void);
void			execute_cmd(char *file, char **cmd_argv);
void			change_dir(char *path);

#endif