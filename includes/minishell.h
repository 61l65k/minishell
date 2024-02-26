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

# include "libft.h"
# include "minimessages.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_Command
{
	char	*command;
	char	*input_file;
	char	*output_file;
	bool	append;
}			t_Command;

typedef struct ShellState
{
	char	input_string[MAXCOM];
	char	**parsed_args;
	bool	is_piped;
	int		cmd_count;
	int		last_exit_status;
}			t_ShellState;

void		ft_free_exit(t_ShellState *state, const char *errormsg,
				int exitcode);
char		*trim_spaces(char *str);
int			ft_cmdhandler(t_ShellState *state, char **parsed);
void		ft_executecmd(t_ShellState *state);
void		ft_free_resets(t_ShellState *state);
void		ft_displayhistory(void);
int			init_signals(void);
void		setup_terminal(void);
void		ft_parseinput(t_ShellState *state);
void		execute_cmd(char *file, char **cmd_argv);

#endif