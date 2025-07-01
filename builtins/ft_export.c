/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:23:27 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/01 10:02:45 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_valid_identifier(char *key)
{
    int i;

    if (!key || !key[0] || (!ft_isalpha(key[0]) && key[0] != '_'))
        return (0);
    i = 1;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (0);
        i++;
    }
    return (1);
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

void    add_env_node(t_gc *gc, t_env **env, char *key, char *value)
{
    t_env *new_node;
    t_env *tmp;

    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return ;
    new_node->key = gc_strdup(gc, key);
    if (value)
        new_node->value = gc_strdup(gc, value);
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

char *expand_value(t_gc *gc, char *value, t_env *env)
{
    if (!value || value[0] != '$')
        return (gc_strdup(gc, value));
    char *var;
    t_env *node;

    var = value + 1;
    node = find_env_node(env, var);
    if (node && node->value)
        return (gc_strdup(gc, node->value));
    return (gc_strdup(gc, ""));
}

void    ft_export(t_gc *gc, char **args, t_env *env)
{
    char **args_kv;
    t_env *node;
    int i;
    char *key;
    char *value;
    char *expanded;
    int error;

    if (!args[1])
    {
        print_sorted_env(env);
        g_last_exit_status = 0;
        return ;
    }
    i = 1;
    error = 0;
    while (args[i])
    {
        args_kv = ft_split(args[i], '=');
        key = args_kv[0];
        if (args_kv[1])
            value = args_kv[1];
        if (!is_valid_identifier(key))
        {
            write(2, "minishell: export: not a valid identifier\n", 43);
            error = 1;
            free_2d_array(args_kv);
            i++;
            continue;
        }
        node = find_env_node(env, key);
        if (node)
        {
            if (value)
            {
                free(node->value);
                expanded = expand_value(gc, value, env);
                node->value = expanded;
            }
        }
        else
        {
            if (value)
                expanded = expand_value(gc, value, env);
            add_env_node(gc, &env, key, expanded);
        }
        free_2d_array(args_kv);
        i++;
    }
    if (error)
        g_last_exit_status = 1;
    else
        g_last_exit_status = 0;
}
