/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:49:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/25 18:52:22 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *new_command(t_gc *gc)
{
    t_command *cmd = gc_malloc(gc, sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->redir = NULL;
    cmd->next = NULL;
    cmd->heredoc_fd = -1;
    cmd->has_heredoc = 0;
    return cmd;
}

char *strip_quotes_cmd(t_gc *gc, char *s)
{
    size_t len;

    if (!s)
        return NULL;

    len = ft_strlen(s);
    if (len < 2)
        return gc_strdup(gc, s);

    if ((s[0] == '"' && s[len - 1] == '"'))
        return gc_substr(gc, s, 1, len - 2);

    return gc_strdup(gc, s);
}

void add_argument(t_gc *gc, t_command *cmd, char *arg, t_token_type type)
{
    size_t count = 0;
    char *clean_arg;

    if (!cmd || !arg)
        return;

    while (cmd->args && cmd->args[count])
        count++;

    // Don't strip single quotes
    if (type == TOKEN_SQUOTE)
        clean_arg = gc_strdup(gc, arg);
    else
        clean_arg = strip_quotes_cmd(gc, arg);

    cmd->args = gc_realloc(gc, cmd->args, sizeof(char *) * (count + 2));
    cmd->args[count] = clean_arg;
    cmd->args[count + 1] = NULL;
}

void add_redirection(t_gc *gc, t_command *cmd, char *type, char *file)
{
    t_redirection *redir = gc_malloc(gc, sizeof(t_redirection));
    if (!redir)
        return;

    redir->type = gc_strdup(gc, type);

    if (strcmp(type, "<<") == 0)
        redir->file = gc_strdup(gc, file);
    else
        redir->file = strip_quotes_cmd(gc, file);

    redir->next = NULL;

    if (!cmd->redir)
        cmd->redir = redir;
    else
    {
        t_redirection *tmp = cmd->redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = redir;
    }
}

t_command *parse_tokens(t_gc *gc, t_token *tokens)
{
    t_command *head = new_command(gc);
    t_command *current = head;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_ENV ||
            tokens->type == TOKEN_SQUOTE || tokens->type == TOKEN_DQUOTE)
        {
            add_argument(gc, current, tokens->value, tokens->type);
        }
        else if (tokens->type == TOKEN_INPUT && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(gc, current, "<", tokens->value);
        }
        else if (tokens->type == TOKEN_OUTPUT && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(gc, current, ">", tokens->value);
        }
        else if (tokens->type == TOKEN_APPEND && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(gc, current, ">>", tokens->value);
        }
        else if (tokens->type == TOKEN_HEREDOC && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(gc, current, "<<", tokens->value);
        }
        else if (tokens->type == TOKEN_PIPE)
        {
            current->next = new_command(gc);
            current = current->next;
        }
        tokens = tokens->next;
    }
    return head;
}

