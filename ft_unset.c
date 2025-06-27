/*************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:56:15 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/03 23:33:15 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **fill_args_unset(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

void    ft_unset(char *input, t_env *env)
{
    char **args;

    args = fill_args_unset(input);
    while (env)
    {
        if (ft_strcmp(args[1], env->key) == 0)
        {
            free(env->key);
            if (env->value)
                free(env->value);
        }
        env = env->next;
    }
    free_2d_array(args);
}
