/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:12:08 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 21:36:07 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*new_env_node(t_gc *gc, char *key, char *value)
{
	t_env	*node;

	if (!gc || !key)
		return (NULL);
	node = gc_malloc(gc, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = gc_strdup(gc, key);
	if (!node->key)
		return (NULL);
	if (value)
	{
		node->value = gc_strdup(gc, value);
		if (!node->value)
			return (NULL);
	}
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

t_env	*create_env_node(char *env, t_gc *gc)
{
	t_env	*node;
	char	*sep;
	size_t	key_len;

	if (!env || !gc)
		return (NULL);
	sep = ft_strchr(env, '=');
	if (!sep)
		return (NULL);
	key_len = sep - env;
	node = gc_malloc(gc, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = gc_strndup(gc, env, key_len);
	if (!node->key)
		return (NULL);
	node->value = gc_strdup(gc, sep + 1);
	if (!node->value)
		return (NULL);
	node->next = NULL;
	return (node);
}

t_env	*env_init(char **envp, t_gc *gc)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	if (!envp || !gc)
		return (NULL);
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
		if (env->value)
		{
			printf("%s=%s\n", env->key, env->value);
			g_last_exit_status = 0;
		}
		env = env->next;
	}
}
