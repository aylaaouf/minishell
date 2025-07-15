/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:12:08 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 02:59:30 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*new_env_node(t_gc *gc, char *key, char *value)
{
	t_env	*node;

	node = gc_malloc(gc, sizeof(t_env));
	node->key = gc_malloc(gc, ft_strlen(key) + 1);
	ft_strcpy(node->key, key);
	node->value = gc_malloc(gc, ft_strlen(value) + 1);
	ft_strcpy(node->value, value);
	node->next = NULL;
	return (node);
}

t_env	*create_env_node(char *env, t_gc *gc)
{
	t_env	*node;
	char	*sep;
	char	*key;
	char	*value;
	size_t	key_len;

	sep = ft_strchr(env, '=');
	if (!sep)
		return (NULL);
	key_len = sep - env;
	key = gc_malloc(gc, key_len + 1);
	ft_strncpy(key, env, key_len);
	key[key_len] = '\0';
	value = gc_malloc(gc, ft_strlen(sep + 1) + 1);
	ft_strcpy(value, sep + 1);
	node = new_env_node(gc, key, value);
	return (node);
}

t_env	*env_init(char **envp, t_gc *gc)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node(envp[i], gc);
		if (node)
		{
			if (!head)
				head = node;
			else
				tail->next = node;
			tail = node;
		}
		i++;
	}
	return (head);
}

void	print_env(t_env *env)
{
	g_last_exit_status = 1;
	while (env)
	{
		if (env->value && env->value[1])
		{
			printf("%s=%s\n", env->key, env->value);
			g_last_exit_status = 0;
		}
		else
			printf("%s\n", env->key);
		env = env->next;
	}
}
