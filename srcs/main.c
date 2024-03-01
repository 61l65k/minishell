/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/01 10:34:37 by ttakala          ###   ########.fr       */
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
	char	prompt[PATH_MAX + PROMPT_LEN];
	char	*buf;

	ft_strlcpy(prompt, GREEN "➜ " RESET CYAN, PROMPT_LEN);
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

void	reset_state(t_shellstate *state)
{
	ft_memset(state->input_string, 0, sizeof(state->input_string));
	state->parsed_args = NULL;
	state->operators = NULL;
	state->cmd_count = 0;
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
		reset_state(&state);
		if (ft_takeinput(&state) == SUCCESS)
		{
			if (ft_parseinput(&state) != SUCCESS)
				continue ;
			state.last_exit_status = ft_executecmd(&state);
			ft_free_resets(&state);
		}
	}
	ft_free_exit(&state, NULL, EXIT_SUCCESS);
	return (0);
}
