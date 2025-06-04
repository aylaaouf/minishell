/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:47:11 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/04 19:13:33 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_pipe(t_command *cmnds, t_env *env)
{
    int prev_fd;
    int fd[2];
    pid_t pid;
    char **args_env;
    t_command *cmd;
    char *path;

    cmd = cmnds;
    args_env = list_to_array(env);
    prev_fd = -1;
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
            if (prev_fd != -1)
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
            path = find_cmnd_path(cmd->args[0], env);
            if (!path)
            {
                perror("command nor found");
                exit(127);
            }
            execve(path, cmd->args, args_env);
            exit(1);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd->next)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        cmd = cmd->next;
    }
    while (wait(NULL) > 0);
    free_2d_array(args_env);
    return (0);
}