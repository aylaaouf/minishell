#include "minishell.h"

static int heredoc_pipe(const char *delimiter, t_env *env)
{
    int pipefd[2];
    char *line;
    char *expanded_line;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
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
        // توسعة المتغيرات قبل الكتابة في pipe
        expanded_line = expand_env(line, env);
        free(line);

        write(pipefd[1], expanded_line, strlen(expanded_line));
        write(pipefd[1], "\n", 1);
        free(expanded_line);
    }
    close(pipefd[1]);
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
