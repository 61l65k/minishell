NAME = minishell
CC = cc
SRCSDIR = srcs
SRCS =  /built-in/built-in_cd.c \
 		/built-in/built-in_echo_pwd_env.c \
 		/built-in/built-in_exit.c \
		/built-in/built-in_export.c \
		/built-in/built-in_unset.c \
		/env/env_utils.c \
		/env/ft_getenv.c \
		/env/ft_setenv.c \
		/env/shell_level.c \
		/execution/exec_builtin.c \
		/execution/exec_builtin_io_apply.c \
		/execution/exec_builtin_io_cleanup.c \
		/execution/exec_builtin_io_heredoc.c \
		/execution/exec_builtin_io_parse.c \
		/execution/exec_cmd.c \
		/execution/exec_heredoc.c \
		/execution/exec_main.c \
		/execution/exec_main_utils.c \
		/execution/exec_redirectors.c \
		/execution/exec_wait.c \
		/free_exit.c \
		/main.c \
		/parseinput/input_to_str/parse_char.c \
		/parseinput/input_to_str/parse_char_checks.c \
		/parseinput/input_to_str/parse_char_utils.c \
		/parseinput/parse_main.c \
		/parseinput/parse_syntax_validation.c \
		/parseinput/str_to_lst/parse_lst.c \
		/parseinput/str_to_lst/parse_lst_quoted.c \
		/parseinput/str_to_lst/parse_lst_utils.c \
		/parseinput/str_to_lst/parse_lst_wcard.c \
		/signals.c \
		/utils_enum.c \
		/utils_str_arr.c \

SRCS := $(addprefix $(SRCSDIR), $(SRCS))
LIBFT = libft/libft.a
OBJS = $(SRCS:.c=.o)

# Default flags for macOS (Darwin)
READ_LINE_PATH := ${HOME}/.brew/Cellar/readline/8.2.10
CFLAGS_DARWIN = -g -Werror -Wall -Wextra
LDFLAGS_DARWIN = -lreadline -L${READ_LINE_PATH}/lib
HEADER_DARWIN = -I${READ_LINE_PATH}/include -Iincludes -Ilibft

# Default flags for Linux
CFLAGS_LINUX = -Werror -Wall -Wextra
LDFLAGS_LINUX = -lreadline
HEADER_LINUX = -Iincludes -Ilibft

# Check the operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS := $(CFLAGS_DARWIN)
    LDFLAGS := $(LDFLAGS_DARWIN)
    HEADER := $(HEADER_DARWIN)
else
    CFLAGS := $(CFLAGS_LINUX)
    LDFLAGS := $(LDFLAGS_LINUX)
    HEADER := $(HEADER_LINUX)
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -Llibft -lft $(LDFLAGS) -o $(NAME)
	@printf "\033[0;32m 🐚 $(NAME) succesfully created. 🐚\033[0m\n"


$(LIBFT):
	@make -s -C libft

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADER) -c $< -o $@
	@printf "\033[0;33mCompiling $< 🔨\033[0m\n"

clean:
	@rm -rf $(OBJS)
	@make -s -C libft clean

fclean: clean
	@rm -rf $(NAME)
	@make -s -C libft fclean
	@printf "\033[0;31m Succesfully Cleaned.\033[0m\n"

re: fclean all

.PHONY: all clean fclean re
