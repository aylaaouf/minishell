/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:06:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/09 21:51:51 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
}							t_token_type;

extern int					g_last_exit_status;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_redirection
{
	char					*type;
	char					*file;
	struct s_redirection	*next;
}							t_redirection;

typedef struct s_command
{
	bool					has_heredoc;
	int						heredoc_fd;
	int						saved_stdin;
	int						saved_stdout;
	char					**args;
	t_redirection			*redir;
	struct s_command		*next;
}							t_command;

typedef struct s_echo
{
	int						newline;
	char					**args;
	int						arg_count;
}							t_echo;

typedef struct s_gc_node
{
	void					*ptr;
	struct s_gc_node		*next;
}							t_gc_node;

typedef struct s_gc
{
	t_gc_node				*list;
}							t_gc;

typedef struct s_heredoc_data
{
	t_gc					*gc;
	char					*delim;
	int						expand;
	t_env					*env;
}							t_heredoc_data;

typedef struct s_expand_data
{
	t_gc					*gc;
	char					*result;
	char					*line;
	t_env					*env;
}							t_expand_data;

typedef struct s_parse_context
{
	char					**joined;
	t_token					**tokens;
	t_gc					*gc;
	int						is_after_heredoc;
}							t_parse_context;

typedef struct s_tokenize_params
{
	t_token					**tokens;
	t_gc					*gc;
	int						is_after_heredoc;
}							t_tokenize_params;

// gc.c
void						*gc_malloc(t_gc *gc, size_t size);
char						*gc_strdup(t_gc *gc, const char *s);
void						*gc_realloc(t_gc *gc, void *ptr, size_t size);
void						gc_add(t_gc *gc, void *ptr);
void						gc_clear(t_gc *gc);
// errors.c
int							check_syntax(t_token *tokens);
void						sigint_handler(int sig);
void						handle_redirection(t_command *cmd, int prev_fd);
// builtins
void						ft_echo(char **args);
void						ft_cd(t_gc *gc, char **input, t_env *env);
void						ft_pwd(char **input, t_env *env);
void						ft_export(t_gc *gc, char **input, t_env **env);
void						ft_unset(char **input, t_env **env);
int							ft_exit(char **input);
void						builtins(t_gc *gc, char **args, t_env **env);
int							is_builtin(char *cmd);
int							is_valid_identifier(char *key);
t_env						*find_env_node(t_env *env, char *key);
char						*expand_value(t_gc *gc, char *value, t_env *env);
void						add_env_node(t_gc *gc, t_env **env, char *key, char *value);
void	sort_env_array(char **env_arr);
void	update_or_add_env(t_gc *gc, t_env **env, char *key, char *value);
int	handle_export_arg(t_gc *gc, t_env **env, char *arg);
// ft_echo.c
char						*expand_env(t_gc *gc, char *input, t_env *env);
// pipe
int							execute_pipe(t_gc *gc, t_command *cmnds,
								t_env *env);
// heredoc_utils_1.c
void						heredoc_child_handler(int sig);
int							is_quoted(const char *s);
char						*strip_quotes(t_gc *gc, const char *s);
char						*get_env_value_heredoc(char *key, t_env *env);
char						*expand_status(t_gc *gc, char *result, size_t *i);
// heredoc_utils_2.c
void						heredoc_child_process(int pipefd,
								t_heredoc_data *data);
char						*expand_variable(t_expand_data *data, size_t *i);
char						*expand_line(t_gc *gc, char *line, t_env *env);
char						*maybe_expand(t_gc *gc, char *line, int expand,
								t_env *env);
void						write_eof_warning(char *delim);
// heredoc_utils_3.c

int							handle_dollar_variable(char *line, int i,
								char **joined, t_gc *gc);
// heredoc.c
int							process_heredocs(t_gc *gc, t_command *commands,
								t_env *env);
// shell
int							shell(t_gc *gc, t_command *cmnd, t_env *env);
char						*ft_strjoin_env(char *s1, char *s2);
char						*ft_strjoin_free(char *s1, char *s2);
void						free_2d_array(char **args);
char						**list_to_array(t_env *env);
char						*find_cmnd_path(t_gc *gc, char *cmnd, t_env *env);

// parse_cmd.c
t_command					*new_command(t_gc *gc);
char						*strip_quotes_cmd(t_gc *gc, char *s);
t_command					*parse_tokens(t_gc *gc, t_token *tokens);
void						add_redirection(t_gc *gc, t_command *cmd,
								char *type, char *file);
// parse_cmd_utils_1.c
t_command					*new_command(t_gc *gc);
char						*strip_quotes_cmd(t_gc *gc, char *s);
size_t						count_arguments(char **args);
char						*get_clean_argument(t_gc *gc, char *arg,
								t_token_type type);
void						add_argument(t_gc *gc, t_command *cmd, char *arg,
								t_token_type type);
// parse_cmd_utils_2.c
char						*get_redirection_type(t_token_type type);
void						add_redirection(t_gc *gc, t_command *cmd,
								char *type, char *file);
void						append_redirection(t_command *cmd,
								t_redirection *redir);
char						*get_redirect_file(t_gc *gc, char *type,
								char *file);
// expander_utils.c
char						*process_character(t_gc *gc, char *result,
								char *str, size_t *i);
char						*handle_dollar_expansion(t_gc *gc, char *str,
								size_t *i, t_env *env);
char						*extract_var_name(t_gc *gc, char *str, size_t *i);
char						*get_env_value(t_env *env, const char *key);
// expander.c
void						expander_on_commands(t_gc *gc, t_command *commands,
								t_env *env);
char						*expand_token_value(t_gc *gc, char *str,
								t_env *env);
void						expander(t_gc *gc, t_token *tokens, t_env *env);
char						*get_env_value(t_env *env, const char *key);
char						*extract_var_name(t_gc *gc, char *str, size_t *i);

// quote_management.c
char						*remove_outer_quotes(t_gc *gc, char *str,
								char quote);
void						quote_management(t_gc *gc, t_token *tokens);
// tokenize.c

int							handle_dollar_variable(char *line, int i,
								char **joined, t_gc *gc);
t_token						*tokenize(char *line, t_gc *gc);
// tokenize_utils_1.c
int							ft_isspace(char c);
bool						is_operator_char(char c);
t_token						*new_token(t_token_type type, char *value,
								t_gc *gc);
void						add_token(t_token **head, t_token *new_);
int							handle_heredoc_quotes(char *line, int i,
								t_token **tokens, t_gc *gc);

// tokenize_utils_2.c
int							handle_dollar_sign(char *line, int i, char **joined,
								t_gc *gc);
int							handle_empty_single_quote(char *line, int i,
								t_token **tokens, t_gc *gc);

int							process_quote_in_word(char *line, int i,
								t_parse_context *ctx);
int							handle_word_or_quotes(char *line, int i,
								t_tokenize_params *params);

// tokenize_utils_2.c
int							handle_dollar_question(char *line, int i,
								char **joined, t_gc *gc);
int							handle_double_quote(char *line, int i,
								char **joined, t_gc *gc);
int							handle_single_quote(char *line, int i,
								char **joined, t_gc *gc);
int							process_single_quote_content(char *line, int i,
								char **joined, t_gc *gc);
// tokenize_utils_4.c

int							handle_standalone_quotes(char *line, int i,
								t_token **tokens, t_gc *gc);
int							check_if_standalone_quote(char *line, int i,
								char **joined);
// envp.c

void						print_env(t_env *env);
t_env						*new_env_node(t_gc *gc, char *key, char *value);
t_env						*env_init(char **envp, t_gc *gc);
// utils_1.c
size_t						ft_strlen(const char *s);
char						*ft_strcpy(char *dst, const char *src);
char						*ft_strndup(char *s1, size_t n);
char						*ft_strchr(const char *s, int c);
char						*gc_strndup(t_gc *gc, const char *s, size_t n);

// utils_2.c
void						*ft_realloc(void *ptr, size_t new_size);
void						*ft_memcpy(void *dst, const void *src, size_t n);
void						*ft_calloc(size_t count, size_t size);
void						ft_bzero(void *s, size_t n);
void						*ft_memset(void *b, int c, size_t len);

// utils_3.c
int							ft_strcmp(const char *s1, const char *s2);
char						*ft_strcat(char *dest, const char *src);
char						**ft_split(char const *s, char c);
size_t						ft_strlcpy(char *dst, const char *src,
								size_t dstsize);
char						*ft_strncpy(char *dest, char *src, size_t n);
char						*ft_strjoin(char const *s1, char const *s2);
int							ft_isalnum(int c);

// utils_4.c

int							ft_isalpha(int c);
char						*gc_substr(t_gc *gc, const char *s,
								unsigned int start, size_t len);
char						*ft_itoa(int n);
// utils_5.c
char						*gc_strjoin_free_a(t_gc *gc, char *s1, char *s2);
char						*ft_strjoin_char_gc(t_gc *gc, char *s, char c);
char						*ft_itoa_gc(t_gc *gc, int n);
#endif
