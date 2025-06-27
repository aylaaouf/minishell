/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:47:11 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/27 23:52:58 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_pipe(t_gc *gc, t_command *cmnds, t_env *env)
{
    int prev_fd = -1;
    int fd[2];
    pid_t pid;
    t_command *cmd = cmnds;
    char **args_env = list_to_array(env);

    while (cmd)
    {
        if (cmd->next && pipe(fd) == -1)
        {
            perror("pipe");
            return (1);
        }

        pid = fork();
        if (pid == 0)
        {
            if (cmd->heredoc_fd != -1)
            {
                dup2(cmd->heredoc_fd, STDIN_FILENO);
                close(cmd->heredoc_fd);
            }
            else if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (cmd->next)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            char *path = find_cmnd_path(gc, cmd->args[0], env);
            if (!path)
            {
                fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
                exit(127);
            }

            execve(path, cmd->args, args_env);
            perror("execve failed");
            exit(1);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd->next)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }

        if (cmd->heredoc_fd != -1)
            close(cmd->heredoc_fd);

        cmd = cmd->next;
    }
    while (wait(NULL) > 0)
        ;
    free_2d_array(args_env);
    return 0;
}
