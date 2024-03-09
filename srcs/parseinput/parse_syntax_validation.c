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
