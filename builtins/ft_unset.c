/*************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:56:15 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/03 23:33:15 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(char **args, t_env **env)
{
	t_env	*curr;
	t_env	*deleted;
	int		i;

	i = 1;
	g_last_exit_status = 1;
	while (args[i])
	{
		if (*env && (*env)->key && ft_strcmp(args[i], (*env)->key) == 0)
		{
			deleted = *env;
			*env = (*env)->next;
			free(deleted->key);
			if (deleted->value)
				free(deleted->value);
			free(deleted);
		}
		curr = *env;
		while (curr && curr->next)
		{
			if (ft_strcmp(args[i], curr->next->key) == 0)
			{
				deleted = curr->next;
				curr->next = curr->next->next;
				free(deleted->key);
				if (deleted->value)
					free(deleted->value);
				free(deleted);
				break ;
			}
			curr = curr->next;
		}
		i++;
	}
	g_last_exit_status = 0;
}
