/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 04:54:51 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 14:36:33 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtin.h"
# include "libft.h"
# include "minimessages.h"
# include "miniutils.h"
# include "vec/vec.h"
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
}								t_operators;

typedef struct s_shellstate
{
	char						*input_string;
	char						**parsed_args;
	t_list						**parsed_cmds;
	t_operators					*operators;
	int							cmd_count;
	int							operator_count;
	int							last_exit_status;
	bool						is_child_process;
	char						**envp;
	t_vec						pid;
}								t_shellstate;

extern volatile sig_atomic_t	g_signal_flag;

bool							is_valid_syntax(t_shellstate *state);
void							print_syntax_err(const char *token,
									const char *backup);
t_operators						str_to_op(const char *str);
const char						*op_to_str(t_operators op);
void							ft_free_exit(t_shellstate *state,
									const char *errormsg, int exitcode);
int								ft_executecmd(t_shellstate *state);
void							wait_remaining_children(t_shellstate *state);
void							wait_child(t_shellstate *state, pid_t pid);
void							ft_free_resets(t_shellstate *state);
int								init_signals(void);
void							setup_terminal(void);
void							ft_execvp(const char *file, char *const argv[],
									char *const envp[]);
char							*ft_getenv(const char *name,
									char *const envp[]);
void							handle_heredoc(t_redirecthelper *rh,
									char *delim, t_exechelper *eh);
int								update_fds(char *filename, t_redirecthelper *rh,
									bool fd_out);
bool							check_pipedoc(t_shellstate *s, t_exechelper *h);
void							check_operators(t_exechelper *h,
									t_shellstate *s);
void							check_g_signal_flag(t_shellstate *s);
void							set_exit_status(t_shellstate *state,
									int status);
bool							is_spaces(const char *str);
bool							ambiguous_redirect(t_shellstate *s);
void							ft_signal_handler(int signo, siginfo_t *info,
									void *context);
;
#endif