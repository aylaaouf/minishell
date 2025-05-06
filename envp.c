/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:12:08 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/01 12:12:14 by ayelasef         ###   ########.fr       */
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
    t_env	*node;

	node = malloc(sizeof(t_env));
    node->key = strdup(key);
    node->value = strdup(value);
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
	int	i;
	size_t key_len;

	tail= NULL;
	head = NULL;
	i = 0;
    while (envp[i])
    {
        sep = ft_strchr(envp[i], '=');
        if (!sep) 
			continue;
        key_len = sep - envp[i];
        key = ft_strndup(envp[i], key_len);
        value = ft_strdup(sep + 1);
        node = new_env_node(key, value);
        free(key); 
		free(value);
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
		i++;
    }
    return (head);
}
