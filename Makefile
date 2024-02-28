NAME = minishell
CC = cc
SRCS := $(shell find srcs -name '*.c')
LIBFT = libft/libft.a
OBJS = $(SRCS:.c=.o)

# Default flags for macOS (Darwin)
READ_LINE_PATH := $(shell [ -d "${HOME}/homebrew/Cellar/readline/8.2.7" ] && echo "${HOME}/homebrew/Cellar/readline/8.2.7" || echo "${HOME}/.brew/Cellar/readline/8.2.7")
CFLAGS_DARWIN = -Werror -Wall -Wextra 
LDFLAGS_DARWIN = -g -lreadline -L${READ_LINE_PATH}/lib
HEADER_DARWIN = -I${READ_LINE_PATH}/include -Iincludes -Ilibft

# Default flags for Linux
CFLAGS_LINUX = -Werror -Wall -Wextra 
LDFLAGS_LINUX = -g -lreadline
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

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -Llibft -lft $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	rm -rf $(OBJS)
	make -C libft clean

fclean: clean
	rm -rf $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
