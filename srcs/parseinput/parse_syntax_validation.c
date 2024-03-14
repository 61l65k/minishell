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

#include "io_type.h"
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

static bool	is_valid_redirect_syntax(t_list *arg_list, t_operators op)
{
	t_list	*curr;

	curr = arg_list;
	while (curr)
	{
		if (curr->type != IO_NONE)
		{
			if (curr->next == NULL || curr->next->content == NULL)
			{
				print_syntax_err(op_to_str(op), "newline");
				return (false);
			}
			curr = curr->next;
			if (curr->type != IO_NONE)
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

	i = 0;
	while (state->cmd_head[i])
	{
		if (is_valid_redirect_syntax(state->cmd_head[i],
				state->operators[i]) == false)
		{
			state->last_exit_status = 2;
			return (false);
		}
		i++;
	}
	state->last_exit_status = 0;
	return (true);
}

void	handle_tilda(t_parsehelper *h, t_shellstate *state)
{
	const char	*tilda = ft_getenv("HOME", state->envp);
	const int	til_len = ft_strlen(tilda);

	if (tilda)
	{
		ensure_mem_for_cmd(h, state, ft_strlen(h->curr_cmd) + til_len + 1);
		ft_strncat(h->curr_cmd, tilda, til_len);
		h->j = ft_strlen(h->curr_cmd);
	}
}
