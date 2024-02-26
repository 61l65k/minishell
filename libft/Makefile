CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
INCLUDES = -I ./get_next_line -I ./ft_printf

#Styles
HAMMER_ICON = "\xf0\x9f\x94\xa8"

SRCS_FILES = ft_memset \
		ft_bzero \
		ft_memcpy \
		ft_memmove \
		ft_memchr \
		ft_memcmp \
		ft_strlen \
		ft_strlcpy \
		ft_strlcat \
		ft_strchr \
		ft_strrchr \
		ft_strnstr \
		ft_strncmp \
		ft_atoi \
		ft_isalpha \
		ft_isdigit \
		ft_isalnum \
		ft_isascii \
		ft_isprint \
		ft_toupper \
		ft_tolower \
		ft_calloc \
		ft_strdup \
		ft_substr \
		ft_strjoin \
		ft_strtrim \
		ft_split \
		ft_itoa \
		ft_strmapi \
		ft_striteri \
		ft_putchar_fd \
		ft_putstr_fd \
		ft_putendl_fd \
		ft_putnbr_fd \
		ft_atol \
		ft_atof \
		ft_strcmp \
		ft_isenv \
		
BONUS_FILES = 	ft_lstnew_bonus \
	  		ft_lstadd_front_bonus \
	  		ft_lstsize_bonus \
	  		ft_lstlast_bonus \
	  		ft_lstadd_back_bonus \
	  		ft_lstdelone_bonus \
	  		ft_lstclear_bonus \
	  		ft_lstiter_bonus \
	  		ft_lstmap_bonus

PRINTF_FILES = 	ft_printf \
                ft_printf_nbr \
                ft_printf_hex \

GNL_FILES = get_next_line_bonus \
			get_next_line_utils_bonus

# ft_printf
PRINTF_DIR = ./ft_printf/
PRINTF_SRCS = $(addprefix $(PRINTF_DIR), $(addsuffix .c, $(PRINTF_FILES)))
PRINTF_OBJS = $(addprefix $(PRINTF_DIR), $(addsuffix .o, $(PRINTF_FILES)))

# gnl
GNL_DIR = ./get_next_line/
GNL_SRCS = $(addprefix $(GNL_DIR), $(addsuffix .c, $(GNL_FILES)))
GNL_OBJS = $(addprefix $(GNL_DIR), $(addsuffix .o, $(GNL_FILES)))

# libft
SRCS_DIR = ./
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRCS_FILES)))
BONUS_SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(BONUS_FILES)))
OBJS_DIR = ./
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(SRCS_FILES)))
BONUS_OBJ = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(BONUS_FILES)))

all: $(NAME)

$(NAME): $(OBJS) $(BONUS_OBJ) $(GNL_OBJS) $(PRINTF_OBJS)
	@ar rcs $@ $^
	@printf "\033[0;32mLibrary $(NAME) created. 📖\033[0m\n"

%.o: %.c
	@printf "\033[0;33mCompiling  $< 🔨\033[0m\n"
	@gcc $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@rm -f $(OBJS) $(BONUS_OBJ) $(GNL_OBJS) $(PRINTF_OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)

.PHONY: bonus all clean fclean re

