/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:34:45 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/03 20:49:25 by ayelasef         ###   ########.fr       */
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

char *extract_word(char *line, size_t *i)
{
    size_t start = *i;
    while (line[*i] && !is_operator_char(line[*i]) && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\'' && line[*i] != '\"')
        (*i)++;
    return (ft_strndup(&line[start], *i - start));
}

char *extract_quoted(char *line, size_t *i, char quote_char)
{
    size_t start;
    (*i)++;
    start = *i;
    while (line[*i] && line[*i] != quote_char)
        (*i)++;
    if (!line[*i])
        return NULL;
    char *content = strndup(&line[start], *i - start);
    (*i)++;
    return (content);
}

t_token *new_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = strdup(value);
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

t_token *tokenize(char *line)
{
    size_t i;
    t_token *tokens;
	
	tokens= NULL;
	i = 0;
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
        else if (line[i] == '"')
        {
            char *quoted = extract_quoted(line, &i, '"');
            if (!quoted)
                return NULL;
            add_token(&tokens, quoted, TOKEN_DQUOTE);
        }
        else if (line[i] == '\'')
        {
            char *quoted = extract_quoted(line, &i, '\'');
            if (!quoted)
                return NULL;
            add_token(&tokens, quoted, TOKEN_SQUOTE);
        }
        else
        {
            char *word = extract_word(line, &i);
            if (word)
                add_token(&tokens, word, TOKEN_IDINTTFE);
        }
    }
    return (tokens);
}
