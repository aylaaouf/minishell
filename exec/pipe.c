/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:47:11 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 02:40:25 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_prosses_helper(t_command *cmd, t_pipe_data *p)
{
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (p->prev_fd != -1)
	{
		dup2(p->prev_fd, STDIN_FILENO);
		close(p->prev_fd);
	}
	if (cmd->next)
	{
		close(p->fd[0]);
		dup2(p->fd[1], STDOUT_FILENO);
		close(p->fd[1]);
	}
}

void	child_prosses(t_gc *gc, t_command *cmd, t_env *env, t_pipe_data *p)
{
	char	*path;

	child_prosses_helper(cmd, p);
	handle_redirection(cmd, p->prev_fd);
	if (is_builtin(cmd->args[0]))
	{
		builtins(gc, cmd->args, &env);
		gc_clear(gc);
		exit(g_last_exit_status);
	}
	path = find_cmnd_path(gc, cmd->args[0], env);
	if (!path)
	{
		write(2, "minishell: command not found\n", 30);
		exit(127);
	}
	execve(path, cmd->args, list_to_array(env));
	perror("execve failed");
	exit(1);
}

void	execute_pipe_helper(t_command *cmd, t_pipe_data *p)
{
	if (p->prev_fd != -1)
		close(p->prev_fd);
	if (cmd->next)
	{
		close(p->fd[1]);
		p->prev_fd = p->fd[0];
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
}

int	execute_pipe(t_gc *gc, t_command *cmnds, t_env *env)
{
	t_pipe_data	p;
	pid_t		pid;
	pid_t		wpid;
	pid_t		last_pid;
	t_command	*cmd;

	p.prev_fd = -1;
	last_pid = -1;
	wpid = -1;
	cmd = cmnds;
	while (cmd)
	{
		if (cmd->next && pipe(p.fd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == 0)
			child_prosses(gc, cmd, env, &p);
		last_pid = pid;
		execute_pipe_helper(cmd, &p);
		cmd = cmd->next;
	}
	signals_pipe(wpid, last_pid);
	return (g_last_exit_status);
}
