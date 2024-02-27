/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 19:12:29 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 19:12:30 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Ensures that there is enough memory for the command.
 * If not, reallocates double the size of the current memory.
 */
void	ensure_memory_for_cmd(t_parsehelper *h, t_shellstate *state,
		size_t additional_length)
{
	size_t	required_size;
	size_t	new_size;
	char	*new_command;

	required_size = h->j + additional_length + 1;
	if (required_size > h->curr_size)
	{
		new_size = required_size * 2;
		new_command = ft_realloc(h->curr_cmd, h->curr_size, new_size);
		if (!new_command)
			ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
		h->curr_cmd = new_command;
		h->curr_size = new_size;
	}
}
