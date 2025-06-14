/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:16:25 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/14 18:16:27 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int has_quotes(const char *str)
{
    size_t len = strlen(str);

    if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') ||
                     (str[0] == '\"' && str[len - 1] == '\"')))
        return (1);
    return (0);
}

static char *strip_quotes(char *str)
{
    size_t len = strlen(str);

    if (has_quotes(str))
        return ft_strndup(str + 1, len - 2);
    return ft_strdup(str);
}

static int heredoc_pipe(char *delimiter_raw, t_env *env)
{
    int pipefd[2];
    char *line;
    char *expanded_line;
    int expand;
    char *delimiter;

    expand = !has_quotes(delimiter_raw); // Expand only if no quotes
    delimiter = strip_quotes(delimiter_raw);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        free(delimiter);
        return (-1);
    }

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

        if (expand)
            expanded_line = expand_env(line, env);
        else
            expanded_line = ft_strdup(line);

        write(pipefd[1], expanded_line, strlen(expanded_line));
        write(pipefd[1], "\n", 1);

        free(line);
        free(expanded_line);
    }

    close(pipefd[1]);
    free(delimiter);
    return (pipefd[0]);
}

void process_heredocs(t_command *commands, t_env *env)
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
                cmd->heredoc_fd = heredoc_pipe(redir->file, env);
                if (cmd->heredoc_fd == -1)
                {
                    fprintf(stderr, "Error creating heredoc\n");
                    exit(EXIT_FAILURE);
                }
                cmd->has_heredoc = 1;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
}
