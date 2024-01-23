/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:17:20 by apyykone          #+#    #+#             */
/*   Updated: 2024/01/23 12:17:21 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char input[100]; // Buffer for user input

	while (1)
	{
		char *cwd = getcwd(NULL, 0); // Get the current working directory
		if (cwd == NULL)
		{
			perror("getcwd");
			exit(1);
		}

		printf("Current directory: %s\n", cwd);
		free(cwd);

		printf("Enter a command (or 'quit' to exit): ");
		fgets(input, sizeof(input), stdin); // Read user input

		// Remove trailing newline character, if present
		size_t len = strlen(input);
		if (len > 0 && input[len - 1] == '\n')
		{
			input[len - 1] = '\0';
		}

		if (strcmp(input, "quit") == 0)
		{
			printf("Exiting interactive session.\n");
			break ; // Exit the loop if the user enters "quit"
		}
		else
		{
			printf("You entered: %s\n", input);

			// Process the user's input here. You can use functions like sys
		}
	}
}