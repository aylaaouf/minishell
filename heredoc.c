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

static void	heredoc_child_handler(int sig)
{
	(void)sig;
	exit(130);
}

int	is_quoted(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	return ((len >= 2 && s[0] == '\'' && s[len - 1] == '\'') ||
			(len >= 2 && s[0] == '"' && s[len - 1] == '"'));
}

char	*strip_quotes(t_gc *gc, const char *s)
{
	if (is_quoted(s))
		return (gc_substr(gc, s, 1, ft_strlen(s) - 2));
	return (gc_strdup(gc, s));
}

char	*get_env_value_heredoc(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*expand_line(t_gc *gc, char *line, t_env *env)
{
	char	*result = gc_strdup(gc, "");
	size_t	i = 0;

	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			i++;
			if (line[i] == '?')
			{
				char *status = ft_itoa_gc(gc, g_last_exit_status);
				result = gc_strjoin_free_a(gc, result, status);
				i++;
			}
			else
			{
				size_t start = i;
				while (ft_isalnum(line[i]) || line[i] == '_')
					i++;
				char *key = gc_strndup(gc, &line[start], i - start);
				char *val = get_env_value_heredoc(key, env);
				result = gc_strjoin_free_a(gc, result, val);
			}
		}
		else
		{
			result = ft_strjoin_char_gc(gc, result, line[i]);
			i++;
		}
	}
	return (result);
}

char	*maybe_expand(t_gc *gc, char *line, int expand, t_env *env)
{
	if (!expand)
		return (gc_strdup(gc, line));
	return (expand_line(gc, line, env));
}

static int	do_heredoc(t_gc *gc, const char *raw_delim, t_env *env)
{
	int		pipefd[2];
	char	*line;
	int		expand;
	char	*delim;
	int		pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);

	delim = strip_quotes(gc, raw_delim);
	expand = !is_quoted(raw_delim);

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
			if (ft_strcmp(line, delim) == 0)
			{
				free(line);
				break ;
			}
			char *processed = maybe_expand(gc, line, expand, env);
			write(pipefd[1], processed, ft_strlen(processed));
			write(pipefd[1], "\n", 1);
			free(line);
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

int	process_heredocs(t_gc *gc, t_command *cmds, t_env *env)
{
	t_command		*cmd;
	t_redirection	*redir;

	cmd = cmds;
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
				int fd = do_heredoc(gc, redir->file, env);
				if (fd == -1)
					return (-1);
				cmd->heredoc_fd = fd;
				cmd->has_heredoc = 1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
