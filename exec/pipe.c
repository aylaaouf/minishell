/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:47:11 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/09 22:48:17 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_pipe(t_gc *gc, t_command *cmnds, t_env *env)
{
	int			prev_fd;
	int			fd[2];
	pid_t		pid;
	int			status;
	pid_t		wpid;
	pid_t		last_pid;
	t_command	*cmd;
	char		**args_env;
	char		*path;

	prev_fd = -1;
	last_pid = -1;
	cmd = cmnds;
	args_env = list_to_array(env);
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		pid = fork();
		if (pid == 0)
		{
			if (cmd->heredoc_fd != -1)
			{
				dup2(cmd->heredoc_fd, STDIN_FILENO);
				close(cmd->heredoc_fd);
			}
			else if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			handle_redirection(cmd, prev_fd);
			if (is_builtin(cmd->args[0]))
			{
				builtins(gc, cmd->args, &env);
				gc_clear(gc);
				exit(g_last_exit_status);
			}
			path = find_cmnd_path(gc, cmd->args[0], env);
			if (!path)
			{
				fprintf(stderr, "minishell: %s: command not found\n",
					cmd->args[0]);
				exit(127);
			}
			execve(path, cmd->args, args_env);
			perror("execve failed");
			exit(1);
		}
		last_pid = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		if (cmd->heredoc_fd != -1)
			close(cmd->heredoc_fd);
		cmd = cmd->next;
	}
	while ((wpid = wait(&status)) > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(status))
				g_last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_last_exit_status = 128 + WTERMSIG(status);
		}
	}
	free_2d_array(args_env);
	return (g_last_exit_status);
}
