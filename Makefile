NAME := minishell

CC := cc 

CFLAGS := -Wall -Wextra -Werror

SRC := main.c utils_1.c utils_2.c utils_3.c envp.c \
	   tokenize.c quote_management.c \
	   expander.c parse_cmd.c ft_echo.c
OBJS := $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRC)  -o $(NAME) -lreadline

clean :
	rm -rf $(OBJS) $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
