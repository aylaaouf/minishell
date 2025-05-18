/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:09 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/18 18:30:11 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    shell(t_command *cmnd, t_env *env)
{
    pid_t child_pid;
    int status;

    (void)env;
    if (!cmnd || !cmnd->args || !cmnd->args[0])
        return (1);
    child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        exit(41);
    }
    else if (child_pid == 0)
    {
        if (execve(cmnd->args[0], cmnd->args, NULL) == -1)
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