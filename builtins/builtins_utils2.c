/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:50:08 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/14 16:19:00 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env_array(char **env_arr)
{
	char	*tmp;
	int		i;
	int		j;

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
}

void	update_or_add_env(t_gc *gc, t_env **env, char *key, char *value)
{
	t_env	*node;
	char	*expanded;

	expanded = NULL;
	node = find_env_node(*env, key);
	if (node)
	{
		if (value)
		{
			gc_free(gc, node->value);
			expanded = expand_value(gc, value, *env);
			node->value = expanded;
		}
	}
	else
	{
		if (value)
			expanded = expand_value(gc, value, *env);
		else
			expanded = gc_strdup(gc, "");
		add_env_node(gc, env, key, expanded);
	}
}

int	handle_export_arg(t_gc *gc, t_env **env, char *arg)
{
	char	**args_kv;
	char	*key;
	char	*value;

	args_kv = ft_split(gc, arg, '=');
	key = args_kv[0];
	value = args_kv[1];
	if (!is_valid_identifier(key))
	{
		write(2, "minishell: export: not a valid identifier\n", 43);
		return (1);
	}
	update_or_add_env(gc, env, key, value);
	return (0);
}
