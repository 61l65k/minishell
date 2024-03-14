/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniutils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:52:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/05 19:51:02 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIUTILS_H
# define MINIUTILS_H
# include "libft.h"
# include <dirent.h>
# include <stdbool.h>
# ifdef __darwin__
#  include <sys/dirent.h>
# endif
# include <unistd.h>

typedef struct s_shellstate	t_shellstate;
typedef enum e_operators	t_operators;

enum						e_returnvalues
{
	SUCCESS = 0,
	FAILURE = 1,
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
	int						paren_depth;
}							t_operatorhelper;

typedef struct s_exechelper
{
	char					**cmd_arr;
	int						pipefd[2];
	int						fd_in;
	int						i;
	bool					pipe_doc;
	const t_list			*curr_cmd;
}							t_exechelper;

typedef struct s_envhelper
{
	char					*var_name;
	int						var_name_len;
	char					*var_value;
	bool					free_var_value;
	int						val_len;
	int						flags;
}							t_envhelper;

typedef struct s_redirecthelper
{
	int						fd;
	int						last_out_fd;
	int						last_in_fd;
	int						flags;
}							t_redirecthelper;

typedef struct s_hdochelper
{
	int						pipe_fds[2];
	char					*delimiter;
	pid_t					pid;
	t_redirecthelper		*rh;
}							t_hdochelper;

typedef struct s_wildcardhelper
{
	DIR						*dir;
	struct dirent			*entry;
	int						match_count;
	t_list					*prev;
}							t_wildcardhelper;

typedef struct s_adjacenthelper
{
	char					*buff;
	size_t					buffer_index;
	size_t					buff_size;
	char					current_quote;
	char					*new_buff;
}							t_adjacenthelper;

typedef struct s_lsthelper
{
	t_wildcardhelper		wcard;
	const char				*start;
	const char				*end;
	size_t					length;
	size_t					i;
	t_list					*head;
	t_list					*current;
	size_t					arg_start;
	char					*arg;
	size_t					arg_len;
	bool					in_quotes;
	t_list					*current_parent;
	bool					head_assigned;
	int						sub_depth;
}							t_lsthelper;

char						**strarrjoin(const char **arr, const char *str);
char						**lst_to_argv(t_list *arg_list);
char						**lst_to_2darray(t_list *list);
char						**ft_strdup_array(const char **arr);
void						free_str_array(char **str_arr);
void						free_and_null_str_array(char ***str_arr_ptr);
int							ft_parseinput(t_shellstate *state);
char						*trim_spaces(const char *str);
int							str_to_lst(const char *str, t_lsthelper *lh,
								t_shellstate *s, int i);
int							ft_isenv_var(int c);
void						parse_cmd_char(t_parsehelper *h,
								t_shellstate *state);
void						ensure_mem_for_cmd(t_parsehelper *h,
								t_shellstate *state, size_t additional_length);
void						init_char_flags(t_envhelper *eh, char *c,
								t_parsehelper *h);
bool						wildcard_match(const char *pattern,
								const char *str);
int							handle_redirect(t_exechelper *eh);
int							ft_checkdollar(t_shellstate *s, t_parsehelper *h);
void						ft_isquotedredirector(t_list *node);
bool						need_handling(t_lsthelper *t, bool check_quoted);
int							handle_quoted(t_lsthelper *t);
t_operators					check_for_op(t_operatorhelper *op,
								t_shellstate *state, int index);
int							check_parentheses(t_operatorhelper *op,
								t_shellstate *s);
void						create_add_node_wcard(t_lsthelper *t, char *data);
bool						is_prev_redirector(const t_list *prev);
void						ensure_mem_cpy_op(t_operatorhelper *op,
								t_operators operator_type, t_shellstate *state);
int							assign_io_type(t_lsthelper *lh, t_list *new_node);
size_t						str_arr_len(const char **arr);
void						handle_tilda(t_parsehelper *h, t_shellstate *state);
void						print_list(t_list *node, int depth);
int							start_sublist(t_lsthelper *lh);
int							end_sublist(t_lsthelper *lh);
int							handle_wildcard(t_lsthelper *lh);
#endif
