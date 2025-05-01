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

char	**get_env(char **env_or)
{
	int		i;
	char	**env;

	i = 0;
	while (env_or[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (env_or[i])
	{
		env[i] = ft_strdup(env_or[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}
