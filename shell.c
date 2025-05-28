/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:09 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/28 18:59:29 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **list_to_array(t_env *env)
{
    int i;
    char **args;
    t_env *tmp;

    i = 0;
    tmp = env;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    args = malloc((i + 1) * sizeof(char *));
    if (!args)
        return (NULL);
    tmp = env;
    i = 0;
    while (tmp)
    {
        args[i] = ft_strjoin_env(tmp->key, tmp->value);
        i++;
        tmp = tmp->next;
    }
    args[i] = NULL;
    return (args);
}

char    *find_cmnd_path(char *cmnd, t_env *env)
{
    char *path_env;
    char **path;
    char *full_path;
    int i;

    path_env = get_env_value(env, "PATH");
    if (!path_env || !cmnd)
        return (NULL);
    path = ft_split(path_env, ':');
    i = 0;
    while (path[i])
    {
        full_path = ft_strjoin(path[i], "/");
        full_path = ft_strjoin_free(full_path, cmnd);
        if (!access(full_path, X_OK))
            return (full_path);
        i++;
    }
    free_2d_array(path);
    return (NULL);
}

int    shell(t_command *cmnd, t_env *env)
{
    pid_t child_pid;
    int status;
    char **args;
    char *path;

    if (!cmnd || !cmnd->args || !cmnd->args[0])
        return (1);
    path = find_cmnd_path(cmnd->args[0], env);
    if (!path)
    {
        printf("%s: command not found\n", cmnd->args[0]);
        return (1);
    }
    child_pid = fork();
    args = list_to_array(env);
    if (child_pid == -1)
    {
        perror("fork");
        exit(41);
    }
    else if (child_pid == 0)
    {   
        if (execve(path, cmnd->args, args) == -1)
        {
            perror("Couldn't execute");
            exit(7);
        }
    }
    else
    {
        wait(&status);
        free_2d_array(args);
    }
    return (0);
}