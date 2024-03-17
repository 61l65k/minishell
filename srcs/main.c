/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/17 12:54:06 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	{
		if (isatty(STDIN_FILENO))
			ft_fprintf(STDERR_FILENO, "exit\n");
		ft_free_exit(state, NULL, EXIT_SUCCESS);
	}
	if (ft_strlen(state->input_string))
		add_history(state->input_string);
	check_g_signal_flag(state);
}

static void	t_shellstate_init(t_shellstate *state, const char **envp)
{
	state->envp = ft_strdup_array(envp);
	if (state->envp == NULL)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	if (vec_new(&state->pid, 10, sizeof(pid_t)) == -1)
		ft_free_exit(state, ERR_MALLOC, EXIT_FAILURE);
	if (isatty(STDIN_FILENO))
	{
		remove_env_var("OLDPWD", state);
	}
}

int	main(int argc, char **argv, const char **envp)
{
	t_shellstate	state;

	(void)argc;
	(void)argv;
	setup_terminal();
	init_signals();
	state = (t_shellstate){0};
	t_shellstate_init(&state, envp);
	(void)envp;
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
