/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/01 12:12:33 by ayelasef         ###   ########.fr       */
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
	TOKEN_IDIntife,         // simple command/arg
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

//envp.c

t_env *env_init(char **envp);
//utils_1.c
size_t	ft_strlen(const char *s);
char	*ft_strndup(char *s1, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
#endif
