/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/05 17:37:15 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Takes input from the user and stores it in state->input_string.
 * & Exits if calls to getcwd or readline fail.
 */
static void	ft_takeinput(t_shellstate *state)
{
	static char	prompt[PATH_MAX + 4];
	char		*buf;

	if (getcwd(prompt, PATH_MAX) == NULL)
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	ft_strlcat(prompt, "\% ", sizeof(prompt));
	buf = readline(prompt);
	if (buf == NULL)
		ft_free_exit(state, NULL, EXIT_FAILURE);
	if (ft_strlen(buf))
		add_history(buf);
	ft_strlcpy(state->input_string, buf, sizeof(state->input_string));
	free(buf);
}

int	main(int argc, char **argv, const char **envp)
{
	t_shellstate	state;

	(void)argc;
	(void)argv;
	setup_terminal();
	init_signals();
	ft_memset(&state, 0, sizeof(state));
	state.envp = ft_strdup_array(envp);
	vec_new(&state.pid, 10, sizeof(pid_t));
	if (!state.envp || state.pid.memory == NULL)
		ft_free_exit(&state, ERR_MALLOC, EXIT_FAILURE);
	printf(CLEAR_SCREEN);
	while (1)
	{
		ft_free_resets(&state);
		ft_takeinput(&state);
		if (ft_parseinput(&state) == SUCCESS)
			state.last_exit_status = ft_executecmd(&state);
	}
	ft_free_exit(&state, NULL, EXIT_SUCCESS);
	return (0);
}
