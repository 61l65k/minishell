/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 11:57:47 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 22:29:39 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"
# include <stdbool.h>

typedef struct s_shellstate	t_shellstate;
typedef struct s_exechelper	t_exechelper;

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
bool			is_pipeline(t_shellstate *state, t_exechelper *helper);
int				builtin_main(t_shellstate *state, t_list *arg_lst);
int				builtin_child(t_shellstate *state, t_exechelper *exechelper);

#endif