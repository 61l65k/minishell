/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/08 16:34:27 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

/**
 * @brief Takes input from the user and stores it in state->input_string.
 * & Exits if getcwd() fail or if readline() returns NULL (=EOF).
 */
static void	ft_takeinput(t_shellstate *state)
{
	static char		prompt[PATH_MAX + sizeof(DBLUE_E RESET_E "$ ") + 1];
	const size_t	len = ft_strlcpy(prompt, DBLUE_E, sizeof(prompt));

	if (getcwd(prompt + len, PATH_MAX) == NULL)
		ft_free_exit(state, ERR_GETCWD, EXIT_FAILURE);
	ft_strncat(prompt, RESET_E "$ ", sizeof(prompt));
	state->input_string = readline(prompt);
	if (state->input_string == NULL)
		ft_free_exit(state, "exit", EXIT_SUCCESS);
	if (ft_strlen(state->input_string))
		add_history(state->input_string);
	check_g_signal_flag(state);
}

int	main(int argc, char **argv, const char **envp)
{
	t_shellstate	state;

	(void)argc;
	(void)argv;
	setup_terminal();
	state = (t_shellstate){0};
	init_signals();
	state.envp = ft_strdup_array(envp);
	vec_new(&state.pid, 10, sizeof(pid_t));
	if (!state.envp || state.pid.memory == NULL)
		ft_free_exit(&state, ERR_MALLOC, EXIT_FAILURE);
	while (1)
	{
		ft_free_resets(&state);
		ft_takeinput(&state);
		if (ft_parseinput(&state) == SUCCESS && is_valid_syntax(&state))
			state.last_exit_status = ft_executecmd(&state);
	}
	ft_free_exit(&state, NULL, EXIT_SUCCESS);
	return (0);
}
