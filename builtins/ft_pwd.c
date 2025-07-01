/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:15:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/01 10:13:47 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_pwd(char **args, t_env *env)
{
    if (args[1])
    {
        g_last_exit_status = 1;
        write(2, "minishell: pwd: too many arguments\n", 36);
    }
    while (env)
    {
        if (ft_strcmp(env->key, "PWD") == 0)
        {
            printf("%s\n", env->value);
            g_last_exit_status = 0;
        }
        env = env->next;
    }
}