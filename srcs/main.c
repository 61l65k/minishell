/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/18 21:50:06 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_sigint_received = 0;

/**
 * @brief Takes the input from the user.
 * & Returns 1 if the input is empty, otherwise 0.
 */
int ft_takeinput(t_ShellState *state) {
    char cwd[1024];
    char prompt[MAX_PROMPT];
    char *buf;

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



/**
 * @brief Handles the built-in commands.
 * & Returns 1 if the command is handled, otherwise 0.
 */
int	ft_cmdhandler(t_ShellState *state, char **parsed)
{
	if (strcmp(parsed[0], "exit") == 0)
		ft_free_exit(state, NULL, EXIT_SUCCESS);
	else if (strcmp(parsed[0], "cd") == 0)
		return (chdir(parsed[1]), 1);
	else if (strcmp(parsed[0], "help") == 0)
		return (printf(HELP_MSG), 1);
	else if (strcmp(parsed[0], "hello") == 0)
		return (printf(HELLO_MSG, getenv("USER")), 1);
	else if (strcmp(parsed[0], "history") == 0)
		return (ft_displayhistory(), 1);
	return (0);
}

/**
 * @brief Splits the input string into commands and arguments.
 * & Trims the leading and trailing spaces from the commands.
 */
static void	ft_parseinput(t_ShellState *state)
{
	int		i;
	char	*trimmed_command;

	i = -1;
	state->parsed_args = ft_split(state->input_string, '|');
	if (!state->parsed_args)
		ft_free_exit(state, ERR_PROCESTRING, EXIT_FAILURE);
	while (state->parsed_args[state->cmd_count])
		state->cmd_count++;
	state->is_piped = (state->cmd_count > 1);
	while (++i < state->cmd_count)
	{
		trimmed_command = trim_spaces(state->parsed_args[i]);
		if (trimmed_command != state->parsed_args[i])
			free(state->parsed_args[i]);
		state->parsed_args[i] = trimmed_command;
	}
}

int main(void) {
    t_ShellState state;

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
    return 0;
}
