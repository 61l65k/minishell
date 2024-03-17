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

#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <stdio.h>

static int	validation_loop(t_shellstate *s, t_parsehelper *ph)
{
	while (s->input_string[ph->i])
	{
		if (s->input_string[ph->i] == '\'' && !ph->in_double_quote)
			ph->in_single_quote = !ph->in_single_quote;
		else if (s->input_string[ph->i] == '"' && !ph->in_single_quote)
			ph->in_double_quote = !ph->in_double_quote;
		if (!ph->in_single_quote && !ph->in_double_quote)
		{
			if (check_parentheses(s, ph) == FAILURE)
				return (FAILURE);
			ph->command_count += (check_for_op(ph, s, -1) != OP_NONE);
		}
		ph->i++;
	}
	if (ph->in_single_quote || ph->in_double_quote)
		return (ft_putstr_fd(ERR_QUOTES, STDERR_FILENO), FAILURE);
	if (ph->parentheses_depth != 0)
		return (ft_putstr_fd(ERR_PARENTHESES, STDERR_FILENO), FAILURE);
	ph->command_count += ph->command_count - 1;
	return (SUCCESS);
}

static int	validate_input(t_shellstate *s, t_parsehelper *ph)
{
	ph->alloc_size = 100;
	ph->command_count = 1;
	if (validation_loop(s, ph) == FAILURE)
		return (FAILURE);
	ph->in_double_quote = false;
	ph->in_double_quote = false;
	ph->j = 0;
	ph->i = -1;
	ph->alloc_size = ft_strlen(s->input_string) + 1;
	ph->commands = ft_calloc(ph->needed_splits + ph->command_count + 2,
			sizeof(char *));
	ph->curr_cmd = ft_calloc(ph->alloc_size, 1);
	if (!ph->commands || !ph->curr_cmd)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	return (SUCCESS);
}

static char	**split_input(t_shellstate *s, t_parsehelper *ph)
{
	if (validate_input(s, ph) != SUCCESS)
		return (NULL);
	while (s->input_string[++ph->i] != '\0')
		parse_character(ph, s);
	ph->curr_cmd[ph->j] = '\0';
	if (ph->j > 0 && ph->cmd_indx < ph->command_count)
	{
		ph->commands[ph->cmd_indx++] = ft_strdup(ph->curr_cmd);
		if (!ph->commands[ph->cmd_indx - 1])
			ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	}
	for (int i = 0; ph->commands[i]; i++)
		printf("before cmd: %s\n", ph->commands[i]);
	ph->commands = separate_parentheses(ph, s);
	printf("cmd count + splits %d\n", ph->command_count + ph->needed_splits);
	for (int i = 0; ph->commands[i]; i++)
		printf("cmd: %s\n", ph->commands[i]);
	return (free(ph->curr_cmd), ph->commands);
}

static int	process_str_to_lst(t_shellstate *s)
{
	int		i;
	t_list	*curr;

	i = -1;
	while (s->parsed_args[++i])
	{
		s->parsed_cmds[i] = str_to_lst(s->parsed_args[i], s);
		/*if (flase)
		{
			if ((i == 0 && is_spaces(s->parsed_args[i], false)))
				return (free_and_null_str_array(&s->parsed_args),
					set_exit_status(s, SUCCESS), SUCCESS);
			print_syntax_err("ERROR", s->parsed_args[i]);
			set_exit_status(s, SYNTAX_ERROR);
			return (free_and_null_str_array(&s->parsed_args), FAILURE);
		}*/
	}
	free_and_null_str_array(&s->parsed_args);
	if (!s->parsed_cmds[0])
		return (set_exit_status(s, SUCCESS), FAILURE);
	for (t_list **lst = s->parsed_cmds; *lst; lst++)
	{
		curr = *lst;
		while (curr)
		{
			printf("the lst: %s\n", (char *)curr->content);
			curr = curr->next;
		}
	}
	printf("\n\n\n\n\n\n");
	return (SUCCESS);
}

int	ft_parseinput(t_shellstate *s)
{
	t_parsehelper	ph;

	ph = (t_parsehelper){0};
	s->parsed_args = split_input(s, &ph);
	if (!s->parsed_args)
		return (set_exit_status(s, SYNTAX_ERROR), FAILURE);
	while (s->parsed_args[s->cmd_count])
		s->cmd_count++;
	s->parsed_cmds = ft_calloc((s->cmd_count + ph.needed_splits) * 2,
			sizeof(t_list *));
	if (!s->parsed_cmds)
		ft_free_exit(s, ERR_MALLOC, EXIT_FAILURE);
	return (process_str_to_lst(s));
}
