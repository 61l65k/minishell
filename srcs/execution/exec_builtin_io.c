/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_io.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:27:40 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/07 22:49:25 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniutils.h"
#include "io_type.h"

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

int	get_cmd_redirections(t_list *cmd, t_vec *io_vec)
{
	t_list	*current_node;
	t_io	io_node;

	current_node = cmd;
	while (current_node)
	{
		ft_memset(&io_node, 0, sizeof(t_io));
		if (!current_node->is_quoted)
		{
			io_node.type = get_io_type(current_node->content);
			if (io_node.type != IO_NONE)
			{
				if (current_node->next == NULL)
					break ;
				io_node.filename = current_node->next->content;
				if (io_node.filename)
				{
					if (vec_push(io_vec, &io_node) == -1)
						return (FAILURE);
				}
			}
		}
		current_node = current_node->next;
	}
	return (SUCCESS);
}

void	remove_io_from_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (get_io_type(args[i]) != IO_NONE)
		{
			i++;
			if (args[i])
				i++;
			continue ;
		}
		args[j] = args[i];
		i++;
		j++;
	}
}

int	get_command(t_list *cmd, t_command *command)
{
	command->args = lst_to_2darray(cmd);
	if (command->args == NULL)
		return (FAILURE);
	if (vec_new(&command->io_vec, 8, sizeof(t_io)) == -1)
	{
		free(command->args);
		return (FAILURE);
	}
	if (get_cmd_redirections(cmd, &command->io_vec) == FAILURE)
	{
		free(command->args);
		return (FAILURE);
	}
	remove_io_from_args(command->args);
	return (SUCCESS);
}

void	free_command(t_command *command)
{
	free(command->args);
	command->args = NULL;
	vec_free(&command->io_vec);
}
