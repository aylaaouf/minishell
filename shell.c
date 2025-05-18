/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:09 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/18 18:57:27 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strjoin_env(char *s1, char *s2)
{
    size_t i;
    size_t j;
    char *array;

    i = 0;
    j = 0;
    array = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
    if (!array)
        return (NULL);
    while (i < ft_strlen(s1))
    {
        array[i] = s1[i];
        i++;
    }
    array[i++] = '=';
    while (j < ft_strlen(s2))
    {
        array[i] = s2[j];
        i++;
        j++;
    }
    array[i] = '\0';
    return (array);
}

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

int    shell(t_command *cmnd, t_env *env)
{
    pid_t child_pid;
    int status;
    char **args;

    if (!cmnd || !cmnd->args || !cmnd->args[0])
        return (1);
    child_pid = fork();
    args = list_to_array(env);
    if (child_pid == -1)
    {
        perror("fork");
        exit(41);
    }
    else if (child_pid == 0)
    {
        if (execve(cmnd->args[0], cmnd->args, args) == -1)
        {
            perror("Couldn't execute");
            exit(7);
        }
    }
    else
    {
        wait(&status);
    }
    return (0);
}