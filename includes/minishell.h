/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:54:51 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 11:11:50 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "minimessages.h"
# include "miniutils.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define PROMPT_LEN 100

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
}				t_operators;

typedef struct s_shellstate
{
	char		input_string[ARG_MAX];
	char		**parsed_args;
	t_list		**parsed_cmds;
	t_operators	*operators;
	int			cmd_count;
	int			operator_count;
	int			last_exit_status;
	bool		is_child_process;
	char		**envp;
	t_vec		pid;
}				t_shellstate;

const char		*op_to_str(t_operators op);
void			ft_free_exit(t_shellstate *state, const char *errormsg,
					int exitcode);
int				ft_builtin_cmdhandler(t_shellstate *state, t_exechelper *h,
					bool child_process);
int				ft_executecmd(t_shellstate *state);
void			wait_remaining_children(t_shellstate *state);
void			wait_child(t_shellstate *state, pid_t pid);
void			ft_free_resets(t_shellstate *state);
void			ft_displayhistory(void);
int				init_signals(void);
void			setup_terminal(void);
void			ft_execvp(const char *file, char *const argv[],
					char *const envp[]);
char			*ft_getenv(const char *name, char *const envp[]);
int				get_terminal_dimension(bool get_width);

#endif