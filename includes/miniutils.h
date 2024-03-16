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
# include "minimessages.h"
# include <dirent.h>
# include <stdbool.h>
# ifdef __darwin__
#  include <sys/dirent.h>
# endif
# include <unistd.h>

typedef struct s_shellstate	t_shellstate;
typedef enum e_operators	t_operators;
typedef enum e_subtype		t_subtype;
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

typedef struct s_exechelper
{
	char					**cmd_argv;
	int						pipefd[2];
	int						fd_in;
	int						i;
	bool					pipe_doc;
	const t_list			*curr_cmd;
	int						subshell_exit_status;
}							t_exechelper;

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
	t_list					*new_node;
	size_t					arg_len;
	bool					in_quotes;
}							t_lsthelper;

char						**strarrjoin(const char **arr, const char *str);
char						**lst_to_argv(t_list *arg_list);
char						**lst_to_2darray(t_list *list);
char						**ft_strdup_array(const char **arr);
void						free_str_array(char **str_arr);
void						free_and_null_str_array(char ***str_arr_ptr);
int							ft_parseinput(t_shellstate *state);
char						*trim_spaces(const char *str);
t_list						*str_to_lst(const char *str, t_shellstate *s);
int							ft_isenv_var(int c);
void						parse_character(t_parsehelper *h,
								t_shellstate *state);
void						ensure_mem_for_buff(t_parsehelper *ph,
								t_shellstate *s, size_t additional_length,
								void *buff);
void						init_char_flags(int *flags, char *c,
								t_parsehelper *h);
int							handle_redirect(t_exechelper *eh);
int							ft_checkdollar(t_shellstate *s, t_parsehelper *h);
void						ft_isquotedredirector(t_list *node);
bool						need_handling(t_lsthelper *t, bool check_quoted);
int							handle_quoted(t_lsthelper *t);
t_operators					check_for_op(t_parsehelper *op, t_shellstate *state,
								int index);
int							check_parentheses(int *paren_depth, t_shellstate *s,
								t_parsehelper *ph);
bool						is_prev_redirector(const t_list *prev);
int							assign_node_types(t_lsthelper *lh, t_list *new_node,
								t_subtype subtype);
size_t						str_arr_len(const char **arr);
void						handle_tilda(t_parsehelper *h, t_shellstate *state);
int							handle_wildcard(t_lsthelper *lh);
char						*get_env_var_value(t_shellstate *s,
								t_parsehelper *ph, bool *free_var_value);
int							get_flag(int flags, int bit_position);
t_operators					get_op_type(const char *str, bool use_strstr);
const char					*get_operator_str(int f, char c);
char						*ft_trimparentheses(t_lsthelper *lh, bool start);
void						dup_forward_fd(t_shellstate *s, t_exechelper *eh);
#endif
