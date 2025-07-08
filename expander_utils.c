/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:05:21 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 12:05:34 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*extract_var_name(t_gc *gc, char *str, size_t *i)
{
	size_t	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (gc_strndup(gc, "?", 1));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (gc_strndup(gc, &str[start], *i - start));
}
