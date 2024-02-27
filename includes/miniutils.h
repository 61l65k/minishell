/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniutils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:52:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 04:07:19 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIUTILS_H
# define MINIUTILS_H
# include <stdbool.h>
# include <unistd.h>

typedef struct s_shellstate	t_shellstate;

typedef struct s_redirs
{
	char					*command;
	char					*input_file;
	char					*output_file;
	bool					append;
}							t_redirs;

typedef struct s_parsehelper
{
	bool					in_single_quote;
	bool					in_double_quote;
	int						command_count;
	char					**commands;
	char					*current_command;
	int						command_index;
	size_t					j;
	size_t					i;
	size_t					curr_alloc_size;
}							t_parsehelper;

typedef struct s_exechelper
{
	char					**cmd_args;
	int						pipefd[2];
	int						fd_in;
	int						i;
	int						status;
}							t_exechelper;

typedef struct s_envhelper
{
	char					*var_name;
	int						var_name_size;
	int						var_name_index;
	int						old_var_name_size;
	char					*new_var_name;
	char					*var_value;
	int						value_len;
	char					*new_command;
	size_t					required_size;
}							t_envhelper;

int							ft_parseinput(t_shellstate *state);
char						*trim_spaces(char *str);
int							ft_isenv_var(int c);
void						parse_cmd_char(t_parsehelper *h,
								t_shellstate *state);

#endif
