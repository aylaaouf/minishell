/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:04:26 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/10 13:04:39 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!strncmp(cmd, "echo", 4) && strlen(cmd) == 4)
		return (1);
	if (!strncmp(cmd, "cd", 2) && strlen(cmd) == 2)
		return (1);
	if (!strncmp(cmd, "pwd", 3) && strlen(cmd) == 3)
		return (1);
	if (!strncmp(cmd, "export", 6) && strlen(cmd) == 6)
		return (1);
	if (!strncmp(cmd, "unset", 5) && strlen(cmd) == 5)
		return (1);
	if (!strncmp(cmd, "env", 3) && strlen(cmd) == 3)
		return (1);
	if (!strncmp(cmd, "exit", 4) && strlen(cmd) == 4)
		return (1);
	return (0);
}

void	builtins(t_gc *gc, char **args, t_env **env)
{
	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		ft_cd(gc, args, *env);
	else if (!ft_strcmp(args[0], "env"))
		print_env(*env);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd(args, *env);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(gc, args, env);
	else if (!ft_strcmp(args[0], "export"))
		ft_export(gc, args, env);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args);
}
