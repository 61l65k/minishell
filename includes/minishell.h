#ifndef MINI_SHELL_H
# define MINI_SHELL_H
# define MAXCOM 1000
# define MAXLIST 100
# define MAX_PROMPT 1060

# include "libft.h"
# include "minimessages.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct ShellState
{
	char	input_string[MAXCOM];
	char	**parsed_args;
	bool	is_piped;
	int		cmd_count;
}			t_ShellState;

void		ft_free_exit(t_ShellState *shellState, const char *errorMsg,
				int exitCode);
char		*trim_spaces(char *str);
int			ownCmdHandler(char **parsed);
#endif