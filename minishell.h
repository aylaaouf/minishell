/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/03 20:51:48 by ayelasef         ###   ########.fr       */
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
	TOKEN_IDINTTFE,         // simple command/arg
	TOKEN_PIPE,         // |
	TOKEN_INPUT,        // <
	TOKEN_OUTPUT,       // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_ENV,          // $VAR
	TOKEN_SQUOTE,       // 'string'
	TOKEN_DQUOTE,       // "string"
	TOKEN_WHITESPACE,   // space/tab (optional)
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
void print_env(t_env *env);
//utils_1.c
size_t	ft_strlen(const char *s);
char	*ft_strndup(char *s1, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
#endif
