/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 10:38:04 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Takes the input from the user.
 * & Returns 1 if the input is empty, otherwise 0.
 */
static int	ft_takeinput(t_ShellState *state)
{
	char	cwd[1024];
	char	prompt[MAX_PROMPT];
	char	*buf;

	ft_strlcpy(prompt, GREEN "➜ " RESET CYAN, MAX_PROMPT);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	ft_strlcat(prompt, cwd, MAX_PROMPT);
	ft_strlcat(prompt, " " RESET, MAX_PROMPT);
	buf = readline(prompt);
	if (buf == NULL)
		ft_free_exit(state, NULL, EXIT_FAILURE);
	if (ft_strlen(buf))
		add_history(buf);
	ft_strlcpy(state->input_string, buf, sizeof(state->input_string));
	return (free(buf), 0);
}

int	main(void)
{
	t_ShellState	state;

	setup_terminal();
	init_signals();
	printf(CLEAR_SCREEN);
	while (1)
	{
		ft_memset(&state, 0, sizeof(t_ShellState));
		if (ft_takeinput(&state) == 0)
		{
			ft_parseinput(&state);
			ft_executecmd(&state);
			ft_free_resets(&state);
		}
	}
	ft_free_exit(&state, NULL, EXIT_SUCCESS);
	return (0);
}
