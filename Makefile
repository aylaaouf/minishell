NAME := minishell

CC := cc

CFLAGS := -Wall -Wextra -Werror

SRC := main.c utils/utils_1.c utils/utils_2.c utils/utils_3.c builtins/ft_env.c \
	   tokenize.c quote_management.c \
	   expander.c parse_cmd.c builtins/ft_echo.c builtins/ft_cd.c \
	   builtins/ft_pwd.c builtins/ft_exit.c builtins/ft_export.c pipe.c shell.c builtins/ft_unset.c \
	    gc.c errors.c shell_utils.c heredoc.c utils/utils_4.c redirections.c \
		utils/utils_5.c signals.c expander_utils.c \
		heredoc_utils_1.c heredoc_utils_2.c \
		tokenize_utils_1.c tokenize_utils_2.c tokenize_utils_3.c \
		tokenize_utils_4.c parse_cmd_utils_1.c parse_cmd_utils_2.c
OBJS := $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRC)  -o $(NAME) -lreadline

clean :
	rm -rf $(OBJS) $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
