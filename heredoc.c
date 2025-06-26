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

char *strip_quotes(const char *s)
{
    size_t len = ft_strlen(s);
    if (is_quoted(s))
        return ft_substr(s, 1, len - 2);
    return ft_strdup(s);
}

char *expand_if_needed(char *line, t_env *env, int do_expand, int last_exit_status)
{
    if (!do_expand)
        return line;
    char *expanded = expand_env(line, env, last_exit_status);
    free(line);
    return expanded;
}

static void heredoc_child_handler(int sig)
{
    (void)sig;
    exit(130);
}

static int heredoc_pipe(const char *raw_delim, t_env *env, int last_exit_status)
{
    int pipefd[2];
    char *line;
    int status;
    int pid;

    int do_expand = !is_quoted(raw_delim);
    char *delimiter = strip_quotes(raw_delim);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        free(delimiter);
        return -1;
    }
    status = 0;
    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, heredoc_child_handler);
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
            line = expand_if_needed(line, env, do_expand, last_exit_status);
            write(pipefd[1], line, strlen(line));
            write(pipefd[1], "\n", 1);
            free(line);
        }
        free(delimiter);
        close(pipefd[1]);
        exit(0);
    }
    close(pipefd[1]);
    waitpid(pid, &status, 0);
    
    // Check if child was interrupted by Ctrl+C
    if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
    {
        close(pipefd[0]);
        free(delimiter);
        return -1;
    }
    
    free(delimiter);
    return pipefd[0];
}

int	process_heredocs(t_command *commands, t_env *env, int last_exit_status)
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
                cmd->heredoc_fd = heredoc_pipe(redir->file, env, last_exit_status);
                if (cmd->heredoc_fd == -1)
                {
                    return -1;
                }
                cmd->has_heredoc = 1;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return (0);
}
