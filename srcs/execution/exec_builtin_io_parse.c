/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io_parse.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:27:40 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/09 00:37:41 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniutils.h"
#include "io_type.h"

/**
 * @brief Parses the commands and redirections from arg_list
 * and stores them to the t_command struct passed as a parameter.
 * @return Returns 0 on success, non zero on failure (malloc error)
 */
int	get_command(t_list *arg_list, t_command *command)
{
	if (vec_new(&command->io_vec, 8, sizeof(t_io)) == -1)
	{
		return (FAILURE);
	}
	if (store_redirections_in_vec(&command->io_vec, arg_list) == FAILURE)
	{
		vec_free(&command->io_vec);
		return (FAILURE);
	}
	command->args = lst_to_2darray(arg_list);
	if (command->args == NULL)
	{
		vec_free(&command->io_vec);
		return (FAILURE);
	}
	remove_redirections_from_args(command->args, arg_list);
	command->fd_stdin_backup = -1;
	command->fd_stdout_backup = -1;
	return (SUCCESS);
}

static bool	found_io_node(t_io *io_node, t_list *curr, t_list *next)
{
	io_node->type = IO_NONE;
	io_node->ambiguous_redirect = false;
	io_node->filename = NULL;
	io_node->fd = -1;
	if (curr && next)
	{
		io_node->type = get_io_type(curr->content);
		if (io_node->type == IO_NONE)
			return (false);
		io_node->filename = next->content;
		io_node->ambiguous_redirect = next->ambiguous_redirect;
		return (true);
	}
	return (false);
}

int	store_redirections_in_vec(t_vec *io_vec, t_list *arg_list)
{
	t_list	*curr;
	t_io	io_node;

	curr = arg_list;
	while (curr)
	{
		if (found_io_node(&io_node, curr, curr->next))
		{
			if (vec_push(io_vec, &io_node) == -1)
				return (FAILURE);
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (SUCCESS);
}

void	remove_redirections_from_args(char **args, t_list *arg_list)
{
	int		i;
	int		j;
	t_list	*current_node;

	i = 0;
	j = 0;
	current_node = arg_list;
	while (args[i] && current_node)
	{
		if (get_io_type(args[i]) != IO_NONE)
		{
			current_node = current_node->next;
			i++;
			if (args[i] && current_node)
			{
				current_node = current_node->next;
				i++;
			}
			continue ;
		}
		current_node = current_node->next;
		args[j++] = args[i++];
	}
	args[j] = NULL;
}

t_io_type	get_io_type(const char *str)
{
	if (ft_strcmp(str, ">") == 0)
		return (IO_OUT_TRUNC);
	else if (ft_strcmp(str, ">>") == 0)
		return (IO_OUT_APPEND);
	else if (ft_strcmp(str, "<") == 0)
		return (IO_IN_TRUNC);
	else if (ft_strcmp(str, "<<") == 0)
		return (IO_IN_HEREDOC);
	return (IO_NONE);
}
