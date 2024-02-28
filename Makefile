NAME = nanoshell
CC = gcc
MAIN_SRCS =  $(wildcard srcs/*.c)
PARSE_SRCS = $(wildcard srcs/parseinput/*.c)
EXEC_SRCS= $(wildcard srcs/execution/*.c)
BUILTIN_SRCS = $(wildcard srcs/built-in/*.c)
LIBFT_SRCS = make -C libft
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
PARSE_OBJS = $(PARSE_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
BUILTIN_OBJS = $(BUILTIN_SRCS:.c=.o)

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

all : $(NAME)

$(NAME) : $(MAIN_OBJS) $(LIBFT_OBJS) $(PARSE_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(COMFILE_FLAGS) $(MAIN_OBJS) $(PARSE_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS) -Llibft -lft $(COMFILE_FLAGS) -o $(NAME)

libft/libft.a:
	$(LIBFT_SRCS)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean :
	rm -rf $(MAIN_OBJS) $(LIBFT_OBJS) $(PARSE_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS)
	make -C libft clean

fclean : clean
	rm -rf $(NAME)
	make -C libft fclean

re : fclean all

.PHONY : all clean fclean re
