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

t_command *new_command()
{
    t_command *cmd = gc_malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->redir = NULL;
    cmd->next = NULL;
    cmd->heredoc_fd = -1;
    cmd->has_heredoc = 0;
    return cmd;
}

char *strip_quotes_cmd(char *s)
{
    size_t len = ft_strlen(s);
    if (!s || len < 2)
        return ft_strdup(s);
    if ((s[0] == '"' && s[len - 1] == '"') || (s[0] == '\'' && s[len - 1] == '\''))
        return ft_substr(s, 1, len - 2);
    return ft_strdup(s);
}

void add_argument(t_command *cmd, char *arg)
{
    size_t count = 0;
    while (cmd->args && cmd->args[count])
        count++;

    char *clean_arg = strip_quotes_cmd(arg);

    cmd->args = ft_realloc(cmd->args, sizeof(char *) * (count + 2));
    cmd->args[count] = clean_arg;
    cmd->args[count + 1] = NULL;
}

void add_redirection(t_command *cmd, char *type, char *file)
{
    t_redirection *redir = gc_malloc(sizeof(t_redirection));
    if (!redir)
        return;
    redir->type = ft_strdup(type);

    if (strcmp(type, "<<") == 0)
        redir->file = ft_strdup(file);
    else
        redir->file = strip_quotes_cmd(file);

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

t_command *parse_tokens(t_token *tokens)
{
    t_command *head = new_command();
    t_command *current = head;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_ENV ||
            tokens->type == TOKEN_SQUOTE || tokens->type == TOKEN_DQUOTE)
        {
            add_argument(current, tokens->value);
        }
        else if (tokens->type == TOKEN_INPUT && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(current, "<", tokens->value);
        }
        else if (tokens->type == TOKEN_OUTPUT && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(current, ">", tokens->value);
        }
        else if (tokens->type == TOKEN_APPEND && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(current, ">>", tokens->value);
        }
        else if (tokens->type == TOKEN_HEREDOC && tokens->next)
        {
            tokens = tokens->next;
            add_redirection(current, "<<", tokens->value);
        }
        else if (tokens->type == TOKEN_PIPE)
        {
            current->next = new_command();
            current = current->next;
        }
        tokens = tokens->next;
    }
    return head;
}
