/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parentheses.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:06:41 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/16 21:06:42 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	adjust_index_for_parentheses_check(const char *str, int i,
		bool adjust_next)
{
	if (adjust_next)
	{
		while (str[i] && str[i] == ' ')
			i++;
	}
	else
	{
		while (i >= 0 && str[i] == ' ')
			i--;
		if (i > 0 && (str[i] == '&' || (str[i] == '|' && str[i - 1] == '|')))
			i--;
	}
	return (i);
}

int	check_parentheses(t_shellstate *s, t_parsehelper *ph)
{
	const char	*input = s->input_string;
	const int	i = ph->i;
	const int	prev_index = adjust_index_for_parentheses_check(input, i - 1,
				false);
	const int	next_index = adjust_index_for_parentheses_check(input, i + 1,
				true);

	if (input[i] == '(')
	{
		if (i != 0 && input[prev_index] != '(' && input[prev_index] != ')'
			&& check_for_op(ph, s, prev_index) == OP_NONE
			&& !is_spaces(s->input_string, i))
			return (print_syntax_err("(", NULL), FAILURE);
		ph->parentheses_depth++;
	}
	else if (input[i] == ')')
	{
		if (ph->parentheses_depth <= 0)
			return (print_syntax_err(")", NULL), FAILURE);
		if (input[next_index] != '\0' && input[next_index] != '('
			&& input[next_index] != ')' && check_for_op(ph, s,
				next_index) == OP_NONE)
			return (print_syntax_err(")", NULL), FAILURE);
		ph->parentheses_depth--;
	}
	if ((input[i] == '(' && (input[next_index] == ')'
				|| input[next_index] == '(')) || (input[i] == ')'
			&& (input[prev_index] == '(' || input[prev_index] == ')')))
		ph->needed_splits++;
	return (SUCCESS);
}

static void	split_loop(char *str, t_parsehelper *ph, int *start, char **new_arr)
{
	while (str[ph->i])
	{
		if (str[ph->i] == '\'' && !ph->in_double_quote)
			ph->in_single_quote = !ph->in_single_quote;
		else if (str[ph->i] == '"' && !ph->in_single_quote)
			ph->in_double_quote = !ph->in_double_quote;
		if (!ph->in_single_quote && !ph->in_double_quote)
		{
			if (str[ph->i] == '(')
			{
				if (ph->parentheses_depth > 0)
				{
					new_arr[ph->cmd_indx++] = ft_strndup(str + *start, ph->i
							- *start);
					*start = ph->i;
				}
				ph->parentheses_depth++;
			}
			else if (str[ph->i] == ')')
			{
				ph->parentheses_depth--;
				new_arr[ph->cmd_indx++] = ft_strndup(str + *start, ph->i
						- *start + 1);
				*start = ph->i + 1;
			}
		}
		ph->i++;
	}
}

static char	**split_parentheses_from_cmd(t_shellstate *state, t_parsehelper *ph,
		char *str)
{
	int		start;
	char	**new_arr;

	ph->i = 0;
	ph->j = 0;
	start = 0;
	new_arr = ft_calloc(ph->needed_splits + ph->command_count + 1,
			sizeof(char *));
	ph->cmd_indx = 0;
	if (!new_arr)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	split_loop(str, ph, &start, new_arr);
	if ((size_t)start < ph->i)
		new_arr[ph->cmd_indx++] = ft_strndup(str + start, ph->i - start);
	new_arr[ph->cmd_indx] = NULL;
	return (new_arr);
}

char	**separate_parentheses(t_parsehelper *ph, t_shellstate *s)
{
	char **new_commands;
	char **tmp;
	int i = 0;
	int new_indx = 0;

	new_commands = ft_calloc((ph->command_count + ph->needed_splits) * 2,
			sizeof(char *));
	while (ph->commands[i])
	{
		tmp = split_parentheses_from_cmd(s, ph, ph->commands[i]);
		if (tmp)
		{
			for (int k = 0; tmp[k]; k++)
			{
				if (tmp[k] != NULL)
					new_commands[new_indx++] = ft_strdup(tmp[k]);
			}
		}
		else
			new_commands[new_indx++] = ft_strdup(ph->commands[i]);
		i++;
	}
	new_commands[new_indx] = NULL;
	free(ph->commands);
	return (new_commands);
}