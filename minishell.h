/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/26 02:30:32 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdio.h>
#include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_token_type
{
	TOKEN_WORD,         // simple command/arg
	TOKEN_PIPE,         // |
	TOKEN_INPUT,        // <
	TOKEN_OUTPUT,       // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_ENV,          // $VAR
	TOKEN_SQUOTE,       // 'string'
	TOKEN_DQUOTE,       // "string"
}   t_token_type;

extern int g_last_exit_status;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_redirection
{
    char *type;
    char *file;
    struct s_redirection *next;
} t_redirection;

typedef struct s_command
{
    bool has_heredoc;
    int heredoc_fd;
    char **args;
    t_redirection *redir;
    struct s_command *next;
} t_command;

typedef struct s_echo
{
    int newline;
    char **args;
    int arg_count;
} t_echo;

typedef struct s_gc_node {
    void                *ptr;
    struct s_gc_node    *next;
}   t_gc_node;

typedef struct s_gc {
    t_gc_node *list;
} t_gc;
//gc.c
void    *gc_malloc(t_gc *gc, size_t size);
char    *gc_strdup(t_gc *gc, const char *s);
void    *gc_realloc(t_gc *gc, void *ptr, size_t size);
void    gc_add(t_gc *gc, void *ptr);
void    gc_clear(t_gc *gc);
//errors.c
int check_syntax(t_token *tokens);

void sigint_handler(int sig);

//builtins
void ft_echo(t_gc *gc, char *input, t_env *env, int last_exit_status);
void    ft_cd(char *input, t_env *env);
void    ft_pwd(char *input, t_env *env);
void    ft_export(char *input, t_env *env);
void    ft_unset(char *input, t_env *env);
int     ft_exit(char *input);
//ft_echo.c
char *expand_env(t_gc *gc, char *input, t_env *env, int last_exit_status);
//pipe
int execute_pipe(t_command *cmnds, t_env *env);
//heredoc.c
int process_heredocs(t_gc *gc, t_command *commands, t_env *env, int last_exit_status);
//shell
int    shell(t_command *cmnd, t_env *env);
char    *ft_strjoin_env(char *s1, char *s2);
char    *ft_strjoin_free(char *s1, char *s2);
void    free_2d_array(char **args);
char    **list_to_array(t_env *env);
char    *find_cmnd_path(char *cmnd, t_env *env);

//parse_cmd.c
t_command *new_command(t_gc *gc);
char *strip_quotes_cmd(t_gc *gc, char *s);
t_command *parse_tokens(t_gc *gc, t_token *tokens);
void add_redirection(t_gc *gc, t_command *cmd, char *type, char *file);
void add_argument(t_gc *gc, t_command *cmd, char *arg);
//expander.c
char *expand_token_value(t_gc *gc, char *str, t_env *env, int last_exit_status);
void expander(t_gc *gc, t_token *tokens, t_env *env, int last_exit_status);
char *get_env_value(t_env *env, const char *key);
char *extract_var_name(t_gc *gc, char *str, size_t *i);

//quote_management.c
char *remove_outer_quotes(t_gc *gc, char *str, char quote);
void quote_management(t_gc *gc, t_token *tokens);
//tokenize.c

void skip_spaces(char *line, size_t *i);
int is_operator_char(char c);
t_token *add_token(t_gc *gc, t_token **head, char *value, t_token_type type);
char *extract_argument(t_gc *gc, char *line, size_t *i);
t_token *tokenize(char *line, t_gc *gc);
//envp.c

void print_env(t_env *env);
t_env *new_env_node(t_gc *gc, char *key, char *value);
t_env *env_init(char **envp, t_gc *gc);
//utils_1.c
size_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strndup(char *s1, size_t n);
char	*ft_strchr(const char *s, int c);
char *gc_strndup(t_gc *gc, const char *s, size_t n);

//utils_2.c
void *ft_realloc(void *ptr, size_t new_size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *b, int c, size_t len);

//utils_3.c
int ft_strcmp(const char *s1, const char *s2);
char *ft_strcat(char *dest, const char *src);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char *ft_strncpy(char *dest, char *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
int	ft_isalnum(int c);

//utils_4.c

int	ft_isalpha(int c);
char *gc_substr(t_gc *gc, const char *s, unsigned int start, size_t len);
char	*ft_itoa(int n);
//utils_5.c
char *gc_strjoin_free_a(t_gc *gc, char *s1, char *s2);
char *ft_strjoin_char_gc(t_gc *gc, char *s, char c);
#endif
