/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:16:25 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 13:34:48 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_child_status(int pid, int read_fd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		return (-1);
	}
	return (read_fd);
}

static int	do_heredoc(t_gc *gc, const char *raw_delim, t_env *env)
{
	int				pipefd[2];
	t_heredoc_data	data;
	int				pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	data.gc = gc;
	data.delim = strip_quotes(gc, raw_delim);
	data.expand = !is_quoted(raw_delim);
	data.env = env;
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child_process(pipefd[1], &data);
	}
	close(pipefd[1]);
	return (handle_child_status(pid, pipefd[0]));
}

static int	handle_heredoc_redirection(t_gc *gc, t_command *cmd,
		t_redirection *redir, t_env *env)
{
	int	fd;

	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	fd = do_heredoc(gc, redir->file, env);
	if (fd == -1)
		return (-1);
	cmd->heredoc_fd = fd;
	cmd->has_heredoc = 1;
	return (0);
}

static int	process_command_heredocs(t_gc *gc, t_command *cmd, t_env *env)
{
	t_redirection	*redir;

	cmd->heredoc_fd = -1;
	redir = cmd->redir;
	while (redir)
	{
		if (ft_strcmp(redir->type, "<<") == 0)
		{
			if (handle_heredoc_redirection(gc, cmd, redir, env) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	process_heredocs(t_gc *gc, t_command *cmds, t_env *env)
{
	t_command	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (process_command_heredocs(gc, cmd, env) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}
