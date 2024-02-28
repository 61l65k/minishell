NAME = minishell
CC = gcc
SRCS := $(shell find srcs -name '*.c')
LIBFT = libft/libft.a
OBJS = $(SRCS:.c=.o)

# Default flags for macOS (Darwin)
CFLAGS_DARWIN = -Werror -Wall -Wextra 
COMFILE_FLAGS_DARWIN = -g -lreadline -L${HOME}/homebrew/Cellar/readline/8.2.7/lib
HEADER_DARWIN = -I${HOME}/homebrew/Cellar/readline/8.2.7/include -Iincludes -Ilibft

# Default flags for Linux
CFLAGS_LINUX = -Werror -Wall -Wextra 
COMFILE_FLAGS_LINUX = -g -lreadline
HEADER_LINUX = -Iincludes -Ilibft

# Check the operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS := $(CFLAGS_DARWIN)
    COMFILE_FLAGS := $(COMFILE_FLAGS_DARWIN)
    HEADER := $(HEADER_DARWIN)
else
    CFLAGS := $(CFLAGS_LINUX)
    COMFILE_FLAGS := $(COMFILE_FLAGS_LINUX)
    HEADER := $(HEADER_LINUX)
endif

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(COMFILE_FLAGS) $(OBJS) -Llibft -lft $(COMFILE_FLAGS) -o $(NAME)

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
