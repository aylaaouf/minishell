/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:23:27 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 11:32:15 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_sorted_env(t_gc *gc, t_env *env)
{
	char	**env_arr;
	int		i;

	env_arr = list_to_array(gc, env);
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		printf("declare -x %s\n", env_arr[i]);
		i++;
	}
}

void	ft_export(t_gc *gc, char **args, t_env **env)
{
	int	i;
	int	error;

	if (!args[1])
		return (print_sorted_env(gc, *env), g_last_exit_status = 0, (void)0);
	i = 1;
	error = 0;
	while (args[i])
	{
		if (handle_export_arg(gc, env, args[i]))
			error = 1;
		i++;
	}
	g_last_exit_status = error;
}
