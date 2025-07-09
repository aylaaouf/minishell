/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:50:41 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/09 22:51:04 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**list_to_array(t_env *env)
{
	int		i;
	char	**args;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	args = malloc((i + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		args[i] = ft_strjoin_env(tmp->key, tmp->value);
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}