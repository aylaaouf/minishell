/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:12:08 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/28 06:59:13 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_env(t_env *env)
{
    g_last_exit_status = 1;
    while (env)
    {
        if (env->value)
        {
            printf("%s=%s\n", env->key, env->value);
            g_last_exit_status = 0;
        }
        env = env->next;
    }
}

t_env *new_env_node(t_gc *gc, char *key, char *value)
{
    t_env *node = gc_malloc(gc, sizeof(t_env));
    node->key = gc_malloc(gc, ft_strlen(key) + 1);
    ft_strcpy(node->key, key);
    node->value = gc_malloc(gc, ft_strlen(value) + 1);
    ft_strcpy(node->value, value);
    node->next = NULL;
    return node;
}

t_env *env_init(char **envp, t_gc *gc)
{
    t_env *head = NULL;
    t_env *tail = NULL;
    t_env *node;
    char *key;
    char *value;
    char *sep;
    int i = 0;
    size_t key_len;

    while (envp[i])
    {
        sep = ft_strchr(envp[i], '=');
        if (!sep)
        {
            i++;
            continue;
        }
        key_len = sep - envp[i];
        key = gc_malloc(gc, key_len + 1);
        ft_strncpy(key, envp[i], key_len);
        key[key_len] = '\0';
        value = gc_malloc(gc, ft_strlen(sep + 1) + 1);
        ft_strcpy(value, sep + 1);
        node = new_env_node(gc, key, value);
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
        i++;
    }
    return head;
}
