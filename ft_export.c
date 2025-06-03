/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:23:27 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/03 22:52:45 by aylaaouf         ###   ########.fr       */
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

t_env *find_env_node(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

void    add_env_node(t_env **env, char *key, char *value)
{
    t_env *new_node;
    t_env *tmp;

    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return ;
    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = NULL;
    new_node->next = NULL;
    if (!*env)
    {
	    *env = new_node;
	    return ;
    }
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}

char *expand_value(char *value, t_env *env)
{
    if (!value || value[0] != '$')
        return (ft_strdup(value));
    char *var;
    t_env *node;

    var = value + 1;
    node = find_env_node(env, var);
    if (node && node->value)
        return (ft_strdup(node->value));
    return (ft_strdup(""));
}

void    ft_export(char *input, t_env *env)
{
    char **args;
    char **args_kv;
    t_env *node;
    int i;
    char *key;
    char *value;
    char *expanded;

    args = fill_args_export(input);
    if (!args[1])
    {
        print_sorted_env(env);
        free_2d_array(args);
        return ;
    }
    i = 1;
    while (args[i])
    {
        args_kv = ft_split(args[i], '=');
        key = args_kv[0];
        if (args_kv[1])
            value = args_kv[1];
        node = find_env_node(env, key);
        if (node)
        {
            if (value)
            {
                free(node->value);
                expanded = expand_value(value, env);
                node->value = expanded;
            }
        }
        else
            add_env_node(&env, key, expanded);
        i++;
    }
    free_2d_array(args);
    free_2d_array(args_kv);
}
