/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:22:30 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/09 14:45:26 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "miniutils.h"
#include "io_type.h"

void	print_syntax_err(const char *token, const char *backup)
{
	if (token && *token)
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", token);
	else if (backup && *backup)
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", backup);
	else
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
}

static bool	is_valid_cmd_count(t_shellstate *state)
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
		print_syntax_err(state->parsed_cmds[i + 1]->content, NULL);
		return (false);
	}
	if (state->operators[i] != OP_NONE)
	{
		print_syntax_err(op_to_str(state->operators[i]), NULL);
		return (false);
	}
	return (true);
}

static bool	is_valid_redirect_syntax(t_list *arg_list)
{
	t_list		*curr;

	curr = arg_list;
	while (curr)
	{
		if (curr->is_quoted_redirector == false
			&& get_io_type(curr->content) != IO_NONE)
		{
			if (curr->next == NULL || curr->next->content == NULL)
			{
				print_syntax_err("newline", NULL);
				return (false);
			}
			curr = curr->next;
			if (get_io_type(curr->content) != IO_NONE)
			{
				print_syntax_err(curr->content, "newline");
				return (false);
			}
		}
		curr = curr->next;
	}
	return (true);
}

bool	is_valid_syntax(t_shellstate *state)
{
	int	i;

	if (is_valid_cmd_count(state) == false)
		return (false);
	i = 0;
	while (state->parsed_cmds[i])
	{
		if (is_valid_redirect_syntax(state->parsed_cmds[i]) == false)
			return (false);
		i++;
	}
	return (true);
}
