NAME		=	minishell
PATH_M		=	./srcs/
HEADER		=	-I./includes -I./libft
LIBS		=	./libft/libft.a
CFLAG =			-g -lm -Wall -Wextra -Werror -lreadline
MANDATORY	=	main.c signals.c utils.c commands.c execute_cmd.c
OBJS_MANDATORY = $(addprefix $(PATH_M), $(MANDATORY:.c=.o))

all: $(NAME)


$(NAME): ${OBJS_MANDATORY}
	@make -s -C ./libft
	@cc ${OBJS_MANDATORY} ${LIBS} ${CFLAG} -o ${NAME} ${HEADER}
	@printf "\033[0;32m📟$(NAME) succesfully created.📟 \033[0m\n"

%.o: %.c
	@cc -Wall -Wextra -Werror -c $< -o $@ ${HEADER}

clean:
	@rm -f ${OBJS_MANDATORY} ${OBJS_BONUS}
	@make clean -s -C ./libft



fclean: clean
	@rm -f ${NAME}
	@rm -f ${LIBS}
	@printf "\033[0;31m Succesfully Cleaned.\033[0m\n"

re: fclean all

.PHONY: all clean fclean re