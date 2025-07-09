/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:03:41 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 22:53:01 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*new_command(t_gc *gc)
{
	t_command	*cmd;

	cmd = gc_malloc(gc, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	cmd->heredoc_fd = -1;
	cmd->has_heredoc = 0;
	return (cmd);
}

char	*strip_quotes_cmd(t_gc *gc, char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len < 2)
		return (gc_strdup(gc, s));
	if ((s[0] == '"' && s[len - 1] == '"'))
		return (gc_substr(gc, s, 1, len - 2));
	return (gc_strdup(gc, s));
}

size_t	count_arguments(char **args)
{
	size_t	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

char	*get_clean_argument(t_gc *gc, char *arg, t_token_type type)
{
	if (type == TOKEN_SQUOTE)
		return (gc_strdup(gc, arg));
	else
		return (strip_quotes_cmd(gc, arg));
}

void	add_argument(t_gc *gc, t_command *cmd, char *arg, t_token_type type)
{
	size_t	count;
	char	*clean_arg;

	if (!cmd || !arg)
		return ;
	count = count_arguments(cmd->args);
	clean_arg = get_clean_argument(gc, arg, type);
	cmd->args = gc_realloc(gc, cmd->args, sizeof(char *) * (count + 2));
	cmd->args[count] = clean_arg;
	cmd->args[count + 1] = NULL;
}
