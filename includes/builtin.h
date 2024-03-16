/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 11:57:47 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/08 11:51:19 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"
# include <stdbool.h>

typedef struct s_shellstate	t_shellstate;
typedef struct s_exechelper	t_exechelper;
typedef struct s_command	t_command;

typedef enum e_builtin_type
{
	BI_NOT_BUILTIN,
	BI_EMPTY,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT
}				t_builtin_type;

void			builtin_echo(char **args, t_shellstate *state);
void			builtin_cd(char **args, t_shellstate *state);
void			builtin_pwd(char **args, t_shellstate *state);
void			builtin_export(char **args, t_shellstate *state);
void			builtin_unset(char **args, t_shellstate *state);
void			builtin_env(char **args, t_shellstate *state);
void			builtin_exit(char **args, t_shellstate *state);

typedef void				(*t_builtin_func)(char **args, t_shellstate *state);
t_builtin_func	get_builtin_func(t_builtin_type type);
t_builtin_type	get_builtin_type(const char *arg);

int				builtin_main(t_shellstate *state, t_list *arg_lst);
int				get_command(t_list *arg_list, t_command *command);
void			free_command(t_command *command);
int				apply_main_process_redirections(t_command *command);
int				restore_main_process_fds(t_command *command);
bool			is_pipeline(t_exechelper *eh);
int				builtin_child(t_shellstate *state, t_exechelper *exechelper);

#endif