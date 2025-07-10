NAME := minishell

CC := cc

CFLAGS := -Wall -Wextra -Werror

SRC := main.c utils/utils_1.c utils/utils_2.c utils/utils_3.c builtins/ft_env.c \
	   parser/tokenize.c parser/quote_management.c \
	   parser/expander.c parser/parse_cmd.c builtins/ft_echo.c builtins/ft_cd.c \
	   builtins/ft_pwd.c builtins/ft_exit.c builtins/ft_export.c builtins/builtins_utils.c builtins/builtins_utils2.c exec/pipe.c exec/shell.c builtins/ft_unset.c \
	    gc.c errors.c exec/shell_utils.c exec/shell_helper.c heredoc.c utils/utils_4.c exec/redirections.c \
		utils/utils_5.c utils/utils_6.c exec/signals.c parser/expander_utils.c \
		heredoc_utils_1.c heredoc_utils_2.c \
		parser/tokenize_utils_1.c parser/tokenize_utils_2.c parser/tokenize_utils_3.c \
		parser/tokenize_utils_4.c parser/parse_cmd_utils_1.c parser/parse_cmd_utils_2.c \
		gc_free.c builtins_check.c
OBJS := $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRC)  -o $(NAME) -lreadline

clean :
	rm -rf $(OBJS) $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
