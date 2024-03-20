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
	t_list		*curr;

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
	while (state->parsed_cmds[i])
	{
		if (is_valid_redirect_syntax(
				state->parsed_cmds[i],
				state->operators[i])
			== false)
		{
			state->last_exit_status = SYNTAX_ERROR;
			return (false);
		}
		i++;
	}
	state->last_exit_status = 0;
	return (true);
}

void	set_exit_status(t_shellstate *state, int status)
{
	state->last_exit_status = status;
}

char	*get_var_value_from_env(t_shellstate *s, t_parsehelper *ph,
		bool *free_var_value)
{
	char	*var_value;
	char	*var_name;
	int		var_name_len;

	var_name_len = ft_envlen((char *)s->input_string + ph->i);
	var_name = ft_strndup(s->input_string + ph->i, var_name_len);
	if (!var_name)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	var_value = ft_getenv(var_name, s->envp);
	ph->i += var_name_len - 1;
	if (var_value == NULL && ph->was_redirect)
	{
		*free_var_value = true;
		ph->was_redirect = false;
		var_value = ft_strjoin(AMBIGUOUS_RESET, var_name);
		if (!var_value)
			return (free(var_name), ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE),
				NULL);
	}
	free(var_name);
	return (var_value);
}
