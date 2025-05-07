/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/07 16:46:06 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdlib.h>

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
	TOKEN_WHITESPACE,   // space/tab
	TOKEN_INVALID
}   t_token_type;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_token
{
	char            *value;         // token string
	t_token_type    type;           // type of the token
	struct s_token  *next;          // linked list
}   t_token;
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
char	*ft_strndup(char *s1, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
//utils_2.c
char *ft_realloc(char *ptr, size_t new_size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *b, int c, size_t len);
//utils_3.c
int ft_strcmp(const char *s1, const char *s2);
char *ft_strcat(char *dest, const char *src);
#endif
