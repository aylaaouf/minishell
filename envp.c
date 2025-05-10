/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:12:08 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 17:42:04 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}

t_env *new_env_node(char *key, char *value)
{
    t_env *node = gc_malloc(sizeof(t_env));
    node->key = gc_malloc(ft_strlen(key) + 1);
    ft_strcpy(node->key, key);
    node->value = gc_malloc(ft_strlen(value) + 1);
    ft_strcpy(node->value, value);
    node->next = NULL;
    return (node);
}

t_env *env_init(char **envp)
{
    t_env *head; 
    t_env *tail;
    t_env *node;
    char *key;
    char *value;
    char *sep;
    int i;
    size_t key_len;

    tail = NULL;
    head = NULL;
    i = 0;
    while (envp[i])
    {
        sep = ft_strchr(envp[i], '=');
        if (!sep) 
        {
            i++;
            continue;
        }
        key_len = sep - envp[i];
        key = gc_malloc(key_len + 1);
        ft_strncpy(key, envp[i], key_len);
        key[key_len] = '\0';
        value = gc_malloc(strlen(sep + 1) + 1);
        ft_strcpy(value, sep + 1);
        node = gc_malloc(sizeof(t_env));
        node->key = key;
        node->value = value;
        node->next = NULL;

        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
        i++;
    }
    return (head);
}
