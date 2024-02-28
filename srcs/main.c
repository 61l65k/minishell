/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/26 19:48:15 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Takes the input from the user.
 * & Returns 1 if the input is empty, otherwise 0.
 */
static int	ft_takeinput(t_shellstate *state)
{
	char	cwd[PATH_MAX];
	char	prompt[PATH_MAX + MAX_PROMPT];
	char	*buf;

	ft_strlcpy(prompt, GREEN "➜ " RESET CYAN, MAX_PROMPT);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	ft_strlcat(prompt, cwd, PATH_MAX);
	ft_strlcat(prompt, " " RESET, PATH_MAX);
	buf = readline(prompt);
	if (buf == NULL)
		ft_free_exit(state, NULL, EXIT_FAILURE);
	if (ft_strlen(buf))
		add_history(buf);
	ft_strlcpy(state->input_string, buf, sizeof(state->input_string));
	return (free(buf), SUCCESS);
}

int	main(void)
{
	t_shellstate	state;
	int				last_exit_status;

	last_exit_status = 0;
	setup_terminal();
	init_signals();
	printf(CLEAR_SCREEN);
	while (1)
	{
		ft_memset(&state, 0, sizeof(t_shellstate));
		state.last_exit_status = last_exit_status;
		if (ft_takeinput(&state) == SUCCESS)
		{
			if (ft_parseinput(&state) != SUCCESS)
				continue ;
			last_exit_status = ft_executecmd(&state);
			ft_free_resets(&state);
		}
	}
	ft_free_exit(&state, NULL, EXIT_SUCCESS);
	return (0);
}
