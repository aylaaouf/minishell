/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:56:15 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 11:12:30 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_node(t_gc *gc, t_env *node)
{
	if (node)
	{
		if (node->key)
			gc_free(gc, node->key);
		if (node->value)
			gc_free(gc, node->value);
		gc_free(gc, node);
	}
}

void	helper(t_gc *gc, t_env **env, char **args, int *i)
{
	t_env	*deleted;

	if (*env && (*env)->key && ft_strcmp(args[*i], (*env)->key) == 0)
	{
		deleted = *env;
		*env = (*env)->next;
		free_env_node(gc, deleted);
	}
}

void	ft_unset(t_gc *gc, char **args, t_env **env)
{
	t_env	*curr;
	t_env	*deleted;
	int		i;

	i = 1;
	g_last_exit_status = 1;
	while (args[i])
	{
		helper(gc, env, args, &i);
		curr = *env;
		while (curr && curr->next)
		{
			if (ft_strcmp(args[i], curr->next->key) == 0)
			{
				deleted = curr->next;
				curr->next = curr->next->next;
				free_env_node(gc, deleted);
				break ;
			}
			curr = curr->next;
		}
		i++;
	}
	g_last_exit_status = 0;
}
