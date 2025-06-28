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

int is_quoted(const char *s)
{
    if (!s)
        return 0;
    size_t len = ft_strlen(s);
    if (len < 2)
        return 0;
    return ((s[0] == '\'' && s[len - 1] == '\'') ||
            (s[0] == '"' && s[len - 1] == '"'));
}

char *strip_quotes(t_gc *gc, const char *s)
{
    size_t len = ft_strlen(s);
    if (is_quoted(s))
        return gc_substr(gc, s, 1, len - 2);
    return gc_strdup(gc, s);
}

char *expand_if_needed(t_gc *gc, char *line, t_env *env, int do_expand, int last_exit_status)
{
    if (!do_expand)
        return line;
    char *expanded = expand_env(gc, line, env, last_exit_status);
    return expanded;
}

static void heredoc_child_handler(int sig)
{
    (void)sig;
    exit(130);
}

static int heredoc_pipe(t_gc *gc, const char *raw_delim, t_env *env, int last_exit_status)
{
    int pipefd[2];
    char *line;
    int status;
    int pid;

    int do_expand = !is_quoted(raw_delim);
    char *delimiter = strip_quotes(gc, raw_delim);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
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
                break;
            if (strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            char *tmp = line;
            line = expand_if_needed(gc, line, env, do_expand, last_exit_status);
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
    return pipefd[0];
}

int process_heredocs(t_gc *gc, t_command *commands, t_env *env, int last_exit_status)
{
    t_command *cmd = commands;
    t_redirection *redir;

    while (cmd)
    {
        cmd->heredoc_fd = -1;
        redir = cmd->redir;
        while (redir)
        {
            if (strcmp(redir->type, "<<") == 0)
            {
                if (cmd->heredoc_fd != -1)
                    close(cmd->heredoc_fd);
                cmd->heredoc_fd = heredoc_pipe(gc, redir->file, env, last_exit_status);
                if (cmd->heredoc_fd == -1)
                    return -1;
                cmd->has_heredoc = 1;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return 0;
}
