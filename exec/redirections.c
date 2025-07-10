/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 05:29:03 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 01:59:42 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redir_in(t_redirection *redir, int prev_fd)
{
	int	fd_in;

	fd_in = open(redir->file, O_RDONLY);
	if (fd_in == -1)
	{
		perror("open");
		g_last_exit_status = 1;
		exit(1);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	if (prev_fd != -1)
	{
		close(prev_fd);
		prev_fd = -1;
	}
}

void	redir_out(t_redirection *redir, int prev_fd)
{
	int	fd_out;

	fd_out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		perror("open");
		g_last_exit_status = 1;
		exit(1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	if (prev_fd != -1)
	{
		close(prev_fd);
		prev_fd = -1;
	}
}

void	redir_out_a(t_redirection *redir, int prev_fd)
{
	int	fd_out_a;

	fd_out_a = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out_a == -1)
	{
		perror("open");
		g_last_exit_status = 1;
		exit(1);
	}
	dup2(fd_out_a, STDOUT_FILENO);
	close(fd_out_a);
	if (prev_fd != -1)
	{
		close(prev_fd);
		prev_fd = -1;
	}
}

void	handle_redirection(t_command *cmd, int prev_fd)
{
	t_redirection	*redir;
	int				saved_stdin;
	int				saved_stdout;

	redir = cmd->redir;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	while (redir)
	{
		if (ft_strcmp(redir->type, "<") == 0)
			redir_in(redir, prev_fd);
		else if (ft_strcmp(redir->type, ">") == 0)
			redir_out(redir, prev_fd);
		else if (ft_strcmp(redir->type, ">>") == 0)
			redir_out_a(redir, prev_fd);
		redir = redir->next;
	}
	cmd->saved_stdin = saved_stdin;
	cmd->saved_stdout = saved_stdout;
}
