/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/28 18:55:06 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdio.h>
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

typedef struct s_gc
{
    void            *ptr;
    struct s_gc     *next;
} t_gc;
extern t_gc *g_gc;
//errors.c
int check_syntax(t_token *tokens);
//ft_echo.c
//builtins
void    ft_echo(char *input, t_env *env);
void    ft_cd(char *input, t_env *env);
void    ft_pwd(char *input, t_env *env);
int     ft_exit(char *input);

//shell
int    shell(t_command *cmnd, t_env *env);
char    *ft_strjoin_env(char *s1, char *s2);
char    *ft_strjoin_free(char *s1, char *s2);
void    free_2d_array(char **args);

//parse_cmd.c
t_command *new_command();
void add_argument(t_command *cmd, char *arg);
void add_redirection(t_command *cmd, char *type, char *file);
t_command *parse_tokens(t_token *tokens);
//expander.c
void expander(t_token *tokens, t_env *env);
char *get_env_value(t_env *env, const char *key);
char *extract_var_name(char *str, size_t *i);
char *expand_token_value(char *str, t_env *env);

//quote_management.c
char *remove_outer_quotes(char *str, char quote);
void quote_management(t_token *tokens);

//tokenize.c
void skip_spaces(char *line, size_t *i);
int is_operator_char(char c);
char *extract_word(char *line, size_t *i);
char *extract_quoted(char *line, size_t *i, char quote_char);
t_token *new_token(char *value, t_token_type type);
t_token *add_token(t_token **head, char *value, t_token_type type);
t_token *tokenize(char *line);

//envp.c
char *get_env_value(t_env *env, const char *key);
t_env *env_init(char **envp);
t_env *new_env_node(char *key, char *value);
void print_env(t_env *env);

//utils_1.c
size_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strndup(char *s1, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);

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
char *ft_strncpy(char *dest, char *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
int	ft_isalnum(int c);

//gc.c
void *gc_malloc(size_t size);
void gc_add(void *ptr);
void gc_free(void *ptr);
void gc_clear(void);
#endif
