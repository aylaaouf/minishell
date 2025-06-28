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

#include "../minishell.h"

char    **fill_args_unset(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

void    ft_unset(char *input, t_env *env)
{
    char **args;
    t_env *curr;
    int     i;
    t_env *deleted;

    args = fill_args_unset(input);
    i = 1;
    g_last_exit_status = 1;
    while (args[i])
    {
        curr = env;
        while (curr && curr->next)
        {
            if (ft_strcmp(args[i], curr->next->key) == 0)
            {
                deleted = curr->next;
                curr->next = curr->next->next;
                free(deleted->key);
                if (deleted->value)
                    free(deleted->value);
                free(deleted);
                g_last_exit_status = 0;
                break ;
            }
            curr = curr->next;
        }
        i++;
    }
    free_2d_array(args);
}
