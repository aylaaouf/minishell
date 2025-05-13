/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:15:52 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/13 19:26:47 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **fill_args_pwd(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

void    ft_pwd(char *input, t_env *env)
{
    char **args;
    args = ft_split(input, ' ');
    if (args[1])
        perror("pwd");
    while (env)
    {
        if (ft_strcmp(env->key, "PWD") == 0)
            printf("%s\n", env->value);
        env = env->next;
    }
}