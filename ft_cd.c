/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:42:28 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/13 17:16:52 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **fill_args_cd(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

char *get_env_value_cd(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

void    update_env_var(t_env *env, char *key, char *new_value)
{
    t_env   *temp = env;

    while (temp)
    {
        if (ft_strcmp(temp->key, key) == 0)
        {
            free(temp->value);
            temp->value = ft_strdup(new_value);
            return ;
        }
        temp = temp->next;
    }
}

void    ft_cd(char *input, t_env *env)
{
    char    **args;
    char    cwd[4096];
    char    *target;

    args = fill_args_cd(input);
    getcwd(cwd, sizeof(cwd));
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
        target = get_env_value_cd(env, "HOME");
    else if (ft_strcmp(args[1], "-") == 0)
        target = get_env_value_cd(env, "OLDPWD");
    else
        target = args[1];
    if (chdir(target) != 0)
    {
        perror("cd");
        return ;
    }
    update_env_var(env, "OLDPWD", cwd);
    getcwd(cwd, sizeof(cwd));
    update_env_var(env, "PWD", cwd);
}