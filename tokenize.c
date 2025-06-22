/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:34:45 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 19:08:31 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void skip_spaces(char *line, size_t *i)
{
    while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
        (*i)++;
}

int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *new_token(char *value, t_token_type type)
{
    t_token *token = gc_malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    return token;
}

t_token *add_token(t_token **head, char *value, t_token_type type)
{
    t_token *new = new_token(value, type);
    if (!new)
        return NULL;

    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    return new;
}

char *extract_argument(char *line, size_t *i)
{
    size_t start = *i;
    char quote;

    while (line[*i] && !is_operator_char(line[*i]) && 
           line[*i] != ' ' && line[*i] != '\t')
    {
        if (line[*i] == '\'' || line[*i] == '"')
        {
            quote = line[(*i)++];
            while (line[*i] && line[*i] != quote)
                (*i)++;
            if (line[*i] == quote)
                (*i)++;
        }
        else
        {
            (*i)++;
        }
    }
    return ft_strndup(&line[start], *i - start);
}

t_token *tokenize(char *line)
{
    size_t i = 0;
    t_token *tokens = NULL;

    while (line[i])
    {
        skip_spaces(line, &i);
        if (!line[i])
            break;

        if (line[i] == '|')
        {
            add_token(&tokens, ft_strdup("|"), TOKEN_PIPE);
            i++;
        }
        else if (line[i] == '>' && line[i + 1] == '>')
        {
            add_token(&tokens, ft_strdup(">>"), TOKEN_APPEND);
            i += 2;
        }
        else if (line[i] == '>')
        {
            add_token(&tokens, ft_strdup(">"), TOKEN_OUTPUT);
            i++;
        }
        else if (line[i] == '<' && line[i + 1] == '<')
        {
            add_token(&tokens, ft_strdup("<<"), TOKEN_HEREDOC);
            i += 2;
        }
        else if (line[i] == '<')
        {
            add_token(&tokens, ft_strdup("<"), TOKEN_INPUT);
            i++;
        }
        else
        {
            char *arg = extract_argument(line, &i);
            if (arg)
                add_token(&tokens, arg, TOKEN_WORD);
        }
    }

    return tokens;
}
