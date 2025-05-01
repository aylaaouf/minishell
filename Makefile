NAME := minishell

CC := cc 

CFLAGS := -Wall -Wextra -Werror

SRC := main.c utils_1.c envp.c
OBJS := $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRC)  -o $(NAME) -lreadline

clean :
	rm -rf $(OBJS) $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
