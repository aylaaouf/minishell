#include "minishell.h"

t_command *new_command()
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->redir = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_command *cmd, char *arg)
{
    size_t count = 0;
    while (cmd->args && cmd->args[count])
        count++;
    cmd->args = ft_realloc(cmd->args, sizeof(char *) * (count + 2));
    cmd->args[count] = ft_strdup(arg);
    cmd->args[count + 1] = NULL;
}

void add_redirection(t_command *cmd, char *type, char *file)
{
    t_redirection *redir = malloc(sizeof(t_redirection));
    if (!redir)
        return ;
    redir->type = ft_strdup(type);
    redir->file = ft_strdup(file);
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
        if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_ENV || tokens->type == TOKEN_SQUOTE || tokens->type == TOKEN_DQUOTE)
            add_argument(current, tokens->value);

        else if (tokens->type == TOKEN_INPUT)
            tokens = tokens->next, add_redirection(current, "<", tokens->value);
        else if (tokens->type == TOKEN_OUTPUT)
            tokens = tokens->next, add_redirection(current, ">", tokens->value);
        else if (tokens->type == TOKEN_APPEND)
            tokens = tokens->next, add_redirection(current, ">>", tokens->value);
        else if (tokens->type == TOKEN_HEREDOC)
            tokens = tokens->next, add_redirection(current, "<<", tokens->value);
        else if (tokens->type == TOKEN_PIPE)
        {
            current->next = new_command();
            current = current->next;
        }
        tokens = tokens->next;
    }
    return (head);
}
