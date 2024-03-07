/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:22:30 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/07 12:16:37 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"

bool	validate_cmd_arr(char *const cmd_arr[])
{
	int			i;
	t_operators	prev;

	prev = str_to_op(cmd_arr[0]);
	i = 0;
	while (cmd_arr[++i])
	{
		if (prev != OP_NONE && str_to_op(cmd_arr[i]) != OP_NONE)
		{
			ft_fprintf(2, "syntax error near unexpected token `%s'\n",
				cmd_arr[i]);
			return (false);
		}
		prev = str_to_op(cmd_arr[i]);
	}
	if (prev != OP_NONE)
	{
		ft_fprintf(2, "syntax error near unexpected token `%s'\n",
			op_to_str(prev));
		return (false);
	}
	return (true);
}

bool	is_valid_cmd_count(t_shellstate *state)
{
	int	i;

	i = 0;
	while (state->parsed_cmds[i] && state->operators[i])
	{
		if (state->operators[i] == OP_NONE)
			break ;
		i++;
	}
	if (state->parsed_cmds[i] && state->parsed_cmds[i + 1])
	{
		ft_fprintf(2, "syntax error near unexpected token `%s'\n",
			state->parsed_cmds[i + 1]->content);
		return (false);
	}
	if (state->operators[i] != OP_NONE)
	{
		ft_fprintf(2, "syntax error near unexpected token `%s'\n",
			op_to_str(state->operators[i]));
		return (false);
	}
	return (true);
}

bool	validate_syntax(t_shellstate *state)
{
	int		i;
	char	**cmd_arr;
	int		result;

	i = 0;
	result = is_valid_cmd_count(state);
	while (i < state->cmd_count && result)
	{
		cmd_arr = lst_to_2darray(state->parsed_cmds[i]);
		if (!cmd_arr)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		result = validate_cmd_arr(cmd_arr);
		free(cmd_arr);
		cmd_arr = NULL;
		i++;
	}
	return (result);
}
