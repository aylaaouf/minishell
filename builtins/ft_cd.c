/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:42:28 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 13:23:27 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value_cd(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	update_env_var(t_gc *gc, t_env *env, char *key, char *new_value)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			temp->value = gc_strdup(gc, new_value);
			return ;
		}
		temp = temp->next;
	}
}

int	ft_helper(char **args)
{
	if (args[1] && args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		g_last_exit_status = 1;
		return (1);
	}
	return (0);
}

void	ft_cd(t_gc *gc, char **args, t_env *env)
{
	char	cwd[4096];
	char	*target;
	char	*home;

	if (ft_helper(args))
		return ;
	getcwd(cwd, sizeof(cwd));
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		target = get_env_value_cd(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		target = get_env_value_cd(env, "OLDPWD");
	else if (args[1][0] == '~')
	{
		home = get_env_value_cd(env, "HOME");
		if (home)
			target = ft_strjoin(gc, home, args[1] + 1);
		else
			target = args[1];
	}
	else
		target = args[1];
	if (!target || chdir(target) != 0)
	{
		perror("minishell: cd");
		g_last_exit_status = 1;
		return ;
	}
	update_env_var(gc, env, "OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	update_env_var(gc, env, "PWD", cwd);
	g_last_exit_status = 0;
}

