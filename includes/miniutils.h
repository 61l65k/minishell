/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniutils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:52:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/04 17:35:16 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIUTILS_H
# define MINIUTILS_H
# include "libft.h"
# include <linux/limits.h>
# include <stdbool.h>
# include <unistd.h>
typedef struct s_shellstate	t_shellstate;
typedef enum e_operators	t_operators;

enum						e_returnvalues
{
	SUCCESS = 0,
	IS_QUOTE = 1,
	FOUNDCMD = 2,
	FOUNDOP = 3,
};

typedef struct s_parsehelper
{
	bool					in_single_quote;
	bool					in_double_quote;
	int						command_count;
	char					**commands;
	char					*curr_cmd;
	int						command_index;
	size_t					j;
	size_t					i;
	size_t					alloc_size;
}							t_parsehelper;

typedef struct s_operatorhelper
{
	bool					in_single_quote;
	bool					in_double_quote;
	size_t					i;
	size_t					ops_i;
	int						cmd_count;
	t_operators				*ops;
	size_t					operators_capacity;
}							t_operatorhelper;

typedef struct s_exechelper
{
	t_list					*cmd_args;
	int						pipefd[2];
	int						fd_in;
	int						i;
	int						status;
	pid_t					pid_current;
}							t_exechelper;

typedef struct s_envhelper
{
	char					*var_name;
	int						var_name_len;
	char					*var_value;
	bool					free_var_value;
	int						val_len;
}							t_envhelper;

typedef struct s_charflags
{
	bool					is_escaped;
	bool					is_env_var;
	bool					is_pipe;
	bool					is_and;
	bool					is_or;
	bool					is_redirect;
	bool					is_input;
	bool					is_append;
	bool					is_heredoc;
}							t_charflags;

typedef struct s_trimhelper
{
	const char				*start;
	const char				*end;
	size_t					length;
	char					*trimmed;
	size_t					i;
	bool					in_single_quote;
	bool					in_double_quote;
	bool					in_quote;
	t_list					*head;
	t_list					*current;
	size_t					arg_start;
	char					*arg;
	t_list					*new_node;
}							t_trimhelper;

char						**strarrjoin(const char **arr, const char *str);
char						**list_to_array(t_list *list);
char						**ft_strdup_array(const char **arr);
void						free_str_array(char **str_arr);
void						free_and_null_str_array(char ***str_arr_ptr);
int							ft_parseinput(t_shellstate *state);
char						*trim_spaces(const char *str);
t_list						*trim_command(const char *str);
int							ft_isenv_var(int c);
void						parse_cmd_char(t_parsehelper *h,
								t_shellstate *state);
void						ensure_mem_for_cmd(t_parsehelper *h,
								t_shellstate *state, size_t additional_length);
int							init_char_flags(t_charflags *flags, char *c,
								t_parsehelper *h);

#endif
