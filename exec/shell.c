/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:09 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/13 01:12:05 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_cmnd_path_helper(t_gc *gc, char **path, char *cmnd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path[i])
	{
		full_path = ft_strjoin(gc, path[i], "/");
		full_path = ft_strjoin_free(gc, full_path, cmnd);
		if (!access(full_path, X_OK))
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmnd_path(t_gc *gc, char *cmnd, t_env *env)
{
	char	*path_env;
	char	**path;
	char	*result;

	if (cmnd[0] == '/' || cmnd[0] == '.')
	{
		if (access(cmnd, F_OK) == -1)
			return (NULL);
		if (access(cmnd, X_OK) == -1)
			return (gc_strdup(gc, cmnd));
		return (gc_strdup(gc, cmnd));
	}
	path_env = get_env_value(env, "PATH");
	if (!path_env || !cmnd)
		return (NULL);
	path = ft_split(gc, path_env, ':');
	if (!path)
		return (NULL);
	result = find_cmnd_path_helper(gc, path, cmnd);
	if (result)
		return (result);
	return (NULL);
}

int	check_cmd_path(t_command *cmd, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			write(2, "minishell: ", 11);
			write(2, cmd->args[0], strlen(cmd->args[0]));
			write(2, ": Is a directory\n", 17);
			g_last_exit_status = 126;
			return (126);
		}
		if (access(path, X_OK) == -1)
		{
			write(2, "minishell: ", 11);
			write(2, cmd->args[0], strlen(cmd->args[0]));
			write(2, ": Permission denied\n", 20);
			g_last_exit_status = 126;
			return (126);
		}
	}
	else
		return (handle_stat_error(cmd));
	return (0);
}

int	shell(t_gc *gc, t_command *cmnd, t_env *env)
{
	pid_t	child_pid;
	int		status;
	char	**args;
	char	*path;
	char	**clean_args;
	int		i;

	if (!cmnd || !cmnd->args)
		return ((g_last_exit_status = 0), 0);
	i = 0;
	while (cmnd->args[i] && !cmnd->args[i][0])
		i++;
	if (!cmnd->args[i])
		return ((g_last_exit_status = 0), 0);
	path = find_cmnd_path(gc, cmnd->args[i], env);
	status = 0;
	clean_args = &cmnd->args[i];
	if (!path)
	{
		is_not_found(cmnd->args[i]);
		return ((g_last_exit_status = 127), 127);
	}
	if (check_cmd_path(cmnd, path) != 0)
		return (g_last_exit_status);
	args = list_to_array(gc, env);
	child_pid = fork();
	if (child_pid == -1)
		return (handle_fork_error());
	else if (child_pid == 0)
		handle_child_process(cmnd, path, args, clean_args);
	else
		handle_parent_process(status, cmnd);
	return (0);
}
