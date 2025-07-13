/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:50:41 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/13 01:13:25 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_fork_error(void)
{
	perror("fork");
	g_last_exit_status = 1;
	return (1);
}

void	handle_child_process(t_command *cmnd, char *path, char **args_env,
		char **clean_args)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (cmnd->heredoc_fd != -1)
	{
		dup2(cmnd->heredoc_fd, STDIN_FILENO);
		close(cmnd->heredoc_fd);
	}
	handle_redirection(cmnd, -1);
	execve(path, clean_args, args_env);
	perror("Couldn't execute");
	exit(127);
}

void	handle_parent_process(int status, t_command *cmnd)
{
	wait(&status);
	if (cmnd->heredoc_fd != -1)
		close(cmnd->heredoc_fd);
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_last_exit_status = 128 + WTERMSIG(status);
}

char	**list_to_array(t_gc *gc, t_env *env)
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
	args = gc_malloc(gc, (i + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		args[i] = ft_strjoin_env(tmp->key, tmp->value, gc);
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}
