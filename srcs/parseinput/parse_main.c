/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 14:43:27 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

static int	validation_loop(t_shellstate *s, t_operatorhelper *op,
							t_parsehelper *ph)
{
	int		paren_depth;

	paren_depth = 0;
	while (s->input_string[ph->i])
	{
		if (s->input_string[ph->i] == '\'' && !ph->in_double_quote)
			ph->in_single_quote = !ph->in_single_quote;
		else if (s->input_string[ph->i] == '"' && !ph->in_single_quote)
			ph->in_double_quote = !ph->in_double_quote;
		if (!ph->in_single_quote && !ph->in_double_quote)
		{
			if (check_parentheses(&paren_depth, s, ph) == FAILURE)
				return (FAILURE);
			ensure_mem_cpy_op(op, check_for_op(ph, s, -1), s);
		}
		ph->i++;
	}
	if (ph->in_single_quote || ph->in_double_quote)
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), FAILURE);
	if (paren_depth != 0)
		return (ft_putstr_fd(ERR_PARENTHESES, STDERR_FILENO), FAILURE);
	return (SUCCESS);
}

static int	count_op_cmds(t_shellstate *s, t_parsehelper *ph)
{
	t_operatorhelper	op;

	op = (t_operatorhelper){0};
	op.operators_capacity = 100;
	op.cmd_count = 1;
	op.ops = ft_calloc(op.operators_capacity, sizeof(t_operators));
	if (!op.ops)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	if (validation_loop(s, &op, ph) == FAILURE)
		return (free(op.ops), FAILURE);
	s->operators = ft_realloc(s->operators, (op.cmd_count - 1)
			* sizeof(t_operators), op.cmd_count * sizeof(t_operators));
	if (!s->operators)
	{
		free(op.ops);
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	}
	ft_memcpy(s->operators, op.ops, op.cmd_count * sizeof(t_operators));
	ph->command_count = op.cmd_count;
	return (free(op.ops), SUCCESS);
}

static char	**split_cmds(t_shellstate *state, t_parsehelper *ph)
{
	if (count_op_cmds(state, ph) != SUCCESS)
		return (NULL);
	ph->in_double_quote = false;
	ph->in_double_quote = false;
	ph->j = 0;
	ph->i = -1;
	ph->alloc_size = ft_strlen(state->input_string) + 1;
	ph->commands = ft_calloc(ph->command_count + 1, sizeof(char *));
	ph->curr_cmd = ft_calloc(ph->alloc_size, 1);
	if (!ph->commands || !ph->curr_cmd)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	while (state->input_string[++ph->i] != '\0')
		parse_character(ph, state);
	ph->curr_cmd[ph->j] = '\0';
	if (ph->j > 0 && ph->command_index < ph->command_count)
	{
		ph->commands[ph->command_index++] = ft_strdup(ph->curr_cmd);
		if (!ph->commands[ph->command_index - 1])
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	}
	if (ph->command_index < ph->command_count)
		ph->commands[ph->command_index] = NULL;
	return (free(ph->curr_cmd), ph->commands);
}

static int	process_str_to_lst(t_shellstate *s)
{
	int	i;

	i = -1;
	while (++i < s->cmd_count)
	{
		s->parsed_cmds[i] = str_to_lst(s->parsed_args[i], s);
		if (s->parsed_cmds[i] == NULL)
		{
			if ((i == 0 && is_spaces(s->parsed_args[i])))
				return (free_and_null_str_array(&s->parsed_args),
					set_exit_status(s, SUCCESS), SUCCESS);
			print_syntax_err(op_to_str(s->operators[i]), s->parsed_args[i]);
			set_exit_status(s, SYNTAX_ERROR);
			return (free_and_null_str_array(&s->parsed_args), FAILURE);
		}
	}
	free_and_null_str_array(&s->parsed_args);
	if (!s->parsed_cmds[0])
		return (set_exit_status(s, SUCCESS), FAILURE);
	return (SUCCESS);
}

int	ft_parseinput(t_shellstate *s)
{
	t_parsehelper	ph;

	ph = (t_parsehelper){0};
	s->parsed_args = split_cmds(s, &ph);
	if (!s->parsed_args)
		return (set_exit_status(s, SYNTAX_ERROR), FAILURE);
	while (s->parsed_args[s->cmd_count])
		s->cmd_count++;
	s->parsed_cmds = ft_calloc(s->cmd_count + 1, sizeof(t_list *));
	if (!s->parsed_cmds)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	return (process_str_to_lst(s));
}
