/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:04:20 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:12:10 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_word_token(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_SQUOTE || type == TOKEN_DQUOTE);
}

char	*get_redirect_file(t_gc *gc, char *type, char *file)
{
	if (ft_strcmp(type, "<<") == 0)
		return (gc_strdup(gc, file));
	else
		return (strip_quotes_cmd(gc, file));
}

void	append_redirection(t_command *cmd, t_redirection *redir)
{
	t_redirection	*tmp;

	if (!cmd->redir)
		cmd->redir = redir;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

void	add_redirection(t_gc *gc, t_command *cmd, char *type, char *file)
{
	t_redirection	*redir;

	redir = gc_malloc(gc, sizeof(t_redirection));
	if (!redir)
		return ;
	redir->type = gc_strdup(gc, type);
	redir->file = get_redirect_file(gc, type, file);
	redir->next = NULL;
	append_redirection(cmd, redir);
}

char	*get_redirection_type(t_token_type type)
{
	if (type == TOKEN_INPUT)
		return ("<");
	else if (type == TOKEN_OUTPUT)
		return (">");
	else if (type == TOKEN_APPEND)
		return (">>");
	else
		return ("<<");
}
