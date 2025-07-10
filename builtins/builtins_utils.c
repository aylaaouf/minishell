/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:07 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 02:54:05 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *key)
{
	int	i;

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

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*expand_value(t_gc *gc, char *value, t_env *env)
{
	char	*var;
	t_env	*node;

	if (!value)
		return (NULL);
	if (value[0] != '$')
		return (gc_strdup(gc, value));
	var = value + 1;
	node = find_env_node(env, var);
	if (node && node->value)
		return (gc_strdup(gc, node->value));
	return (gc_strdup(gc, ""));
}

void	add_env_node(t_gc *gc, t_env **env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = gc_malloc(gc, sizeof(t_env));
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
