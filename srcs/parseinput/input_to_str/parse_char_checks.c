/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_char_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:29:24 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/14 11:29:26 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_tilda(t_parsehelper *h, t_shellstate *state)
{
	const char	*tilda = ft_getenv("HOME", state->envp);
	const int	til_len = ft_strlen(tilda);

	if (tilda)
	{
		ensure_mem_for_buff(h, state, ft_strlen(h->curr_cmd) + til_len + 1,
			h->curr_cmd);
		ft_strncat(h->curr_cmd, tilda, til_len);
		h->j = ft_strlen(h->curr_cmd);
	}
}

int	ft_checkdollar(t_shellstate *s, t_parsehelper *h)
{
	const char	*c = &s->input_string[h->i];

	if (!h->in_single_quote && !h->in_double_quote && (*(c + 1) == ' ' || *(c
				+ 1) == '\0'))
	{
		ensure_mem_for_buff(h, s, 1, h->curr_cmd);
		h->curr_cmd[h->j++] = *c;
		return (1);
	}
	else if ((h->in_single_quote || h->in_double_quote) && (*(c + 1) == '\''
			|| *(c + 1) == '"' || *(c + 1) == ' ' || *(c + 1) == '$'))
	{
		ensure_mem_for_buff(h, s, 1, h->curr_cmd);
		h->curr_cmd[h->j++] = *c;
		return (1);
	}
	return (0);
}

int	get_flag(int flags, int bit_position)
{
	return (flags & (1 << bit_position));
}
