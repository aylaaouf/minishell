/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:23:27 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/02 17:52:13 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **fill_args_export(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

void    print_sorted_env(t_env *env)
{
    char **env_arr;
    char *tmp;
    int i;
    int j;

    env_arr = list_to_array(env);
    i = 0;
    while (env_arr[i])
    {
        j = i + 1;
        while (env_arr[j])
        {
            if (ft_strcmp(env_arr[i], env_arr[j]) > 0)
            {
                tmp = env_arr[i];
                env_arr[i] = env_arr[j];
                env_arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (env_arr[i])
    {
        printf("declare -x ");
        printf("%s\n", env_arr[i++]);
    }
}

void    ft_export(char *input, t_env *env)
{
    char **args;

    args = fill_args_export(input);
    if (!args[1])
        print_sorted_env(env);
}