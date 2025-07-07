/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:16:25 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/25 18:56:22 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value_echo(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*expand_env(t_gc *gc, char *input, t_env *env)
{
	char	*expanded;
	char	*ptr;
	char	*status_str;
	char	*start;
	char	*key;
	char	*val;
			char temp[2] = {*ptr++, '\0'};

	expanded = gc_strdup(gc, "");
	ptr = input;
	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1))
		{
			ptr++;
			if (*ptr == '?')
			{
				status_str = ft_itoa_gc(gc, g_last_exit_status);
				expanded = gc_strjoin_free_a(gc, expanded, status_str);
				ptr++;
			}
			else
			{
				start = ptr;
				while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
					ptr++;
				key = gc_strndup(gc, start, ptr - start);
				val = get_env_value_echo(key, env);
				expanded = gc_strjoin_free_a(gc, expanded, val);
			}
		}
		else
		{
			expanded = gc_strjoin_free_a(gc, expanded, temp);
		}
	}
	return (expanded);
}
int	is_quoted(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 2)
		return (0);
	return ((s[0] == '\'' && s[len - 1] == '\'') || (s[0] == '"' && s[len
			- 1] == '"'));
}

char	*strip_quotes(t_gc *gc, const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (is_quoted(s))
		return (gc_substr(gc, s, 1, len - 2));
	return (gc_strdup(gc, s));
}

char	*expand_if_needed(t_gc *gc, char *line, t_env *env, int do_expand)
{
	char	*expanded;

	if (!do_expand)
		return (line);
	expanded = expand_env(gc, line, env);
	return (expanded);
}

static void	heredoc_child_handler(int sig)
{
	(void)sig;
	exit(130);
}

static int	heredoc_pipe(t_gc *gc, const char *raw_delim, t_env *env)
{
	int		pipefd[2];
	char	*line;
	int		status;
	int		pid;
	int		do_expand;
	char	*delimiter;
	char	*tmp;

	do_expand = !is_quoted(raw_delim);
	delimiter = strip_quotes(gc, raw_delim);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	status = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, heredoc_child_handler);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			tmp = line;
			line = expand_if_needed(gc, line, env, do_expand);
			free(tmp);
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
		}
		close(pipefd[1]);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

int	process_heredocs(t_gc *gc, t_command *commands, t_env *env)
{
	t_command		*cmd;
	t_redirection	*redir;

	cmd = commands;
	while (cmd)
	{
		cmd->heredoc_fd = -1;
		redir = cmd->redir;
		while (redir)
		{
			if (ft_strcmp(redir->type, "<<") == 0)
			{
				if (cmd->heredoc_fd != -1)
					close(cmd->heredoc_fd);
				cmd->heredoc_fd = heredoc_pipe(gc, redir->file, env);
				if (cmd->heredoc_fd == -1)
					return (-1);
				cmd->has_heredoc = 1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
