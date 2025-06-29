/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:09 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/28 05:38:08 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **list_to_array(t_env *env)
{
    int i = 0;
    char **args;
    t_env *tmp = env;

    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }

    args = malloc((i + 1) * sizeof(char *));
    if (!args)
        return NULL;

    tmp = env;
    i = 0;
    while (tmp)
    {
        args[i] = ft_strjoin_env(tmp->key, tmp->value);
        i++;
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

char *find_cmnd_path(t_gc *gc, char *cmnd, t_env *env)
{
    char *path_env;
    char **path;
    char *full_path;
    int i = 0;

    if (cmnd[0] == '/' || cmnd[0] == '.')
    {
        if (!access(cmnd, X_OK))
            return gc_strdup(gc, cmnd);
        return NULL;
    }

    path_env = get_env_value(env, "PATH");
    if (!path_env || !cmnd)
        return NULL;

    path = ft_split(path_env, ':');
    if (!path)
        return NULL;

    while (path[i])
    {
        full_path = ft_strjoin(path[i], "/");
        full_path = ft_strjoin_free(full_path, cmnd);
        if (!access(full_path, X_OK))
        {
            free_2d_array(path);
            return full_path;
        }
        free(full_path);
        i++;
    }

    free_2d_array(path);
    return NULL;
}

int shell(t_gc *gc, t_command *cmnd, t_env *env)
{
    pid_t child_pid;
    int status;
    char **args;
    char *path;

    if (!cmnd || !cmnd->args || !cmnd->args[0])
        return 1;

    path = find_cmnd_path(gc, cmnd->args[0], env);
    if (!path)
    {
        printf("%s: command not found\n", cmnd->args[0]);
        g_last_exit_status = 127;
        return 127;
    }

    args = list_to_array(env);
    child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        g_last_exit_status = 1;
        return 1;
    }
    else if (child_pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
        if (cmnd->heredoc_fd != -1)
        {
            dup2(cmnd->heredoc_fd, STDIN_FILENO);
            close(cmnd->heredoc_fd);
        }
        handle_redirection(cmnd, -1);
        execve(path, cmnd->args, args);
        perror("Couldn't execute");
        exit(127);
    }
    else
    {
        wait(&status);
        free_2d_array(args);
        if (cmnd->heredoc_fd != -1)
            close(cmnd->heredoc_fd);

        if (WIFEXITED(status))
            g_last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_last_exit_status = 128 + WTERMSIG(status);
    }

    return 0;
}
