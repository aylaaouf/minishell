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

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '\'' || c == '"' || c == '$');
}

static t_token	*new_token(t_token_type type, char *value, t_gc *gc)
{
    t_token *token = gc_malloc(gc, sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    return token;
}

static void	add_token(t_token **head, t_token *new)
{
	if (!*head)
		*head = new;
	else {
		t_token *cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

static int	handle_quote(char *line, int i, t_token **tokens, t_gc *gc)
{
	char quote = line[i++];
	int start = i;

	while (line[i] && line[i] != quote)
		i++;

	if (line[i] == quote)
	{
		int len = i - start;
		if (len > 0)
		{
			char *str = gc_strndup(gc, &line[start], len);
			t_token_type type = (quote == '\'') ? TOKEN_SQUOTE : TOKEN_DQUOTE;
			add_token(tokens, new_token(type, str, gc));
		}
		return i + 1;
	}
	return i;
}


static int	handle_operator(char *line, int i, t_token **tokens, t_gc *gc)
{
	if (line[i] == '|' )
		add_token(tokens, new_token(TOKEN_PIPE, gc_strndup(gc, "|", 1), gc));
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(tokens, new_token(TOKEN_HEREDOC, gc_strndup(gc, "<<", 2), gc));
			return i + 2;
		}
		else
			add_token(tokens, new_token(TOKEN_INPUT, gc_strndup(gc, "<", 1), gc));
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(tokens, new_token(TOKEN_APPEND, gc_strndup(gc, ">>", 2), gc));
			return i + 2;
		}
		else
			add_token(tokens, new_token(TOKEN_OUTPUT, gc_strndup(gc, ">", 1), gc));
	}
	return i + 1;
}

static int	handle_dollar(char *line, int i, t_token **tokens, t_gc *gc)
{
	int start = i;
	i++;
	if (line[i] == '?')
	{
		add_token(tokens, new_token(TOKEN_WORD, gc_strndup(gc, &line[start], 2), gc));
		return i + 1;
	}
	int var_start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (i > var_start)
	{
		char *var_name = gc_strndup(gc, &line[var_start], i - var_start);
		char *full_var = gc_strjoin_free_a(gc, gc_strdup(gc, "$"), var_name);
		add_token(tokens, new_token(TOKEN_WORD, full_var, gc));
	}
	else
	{
		add_token(tokens, new_token(TOKEN_WORD, gc_strndup(gc, &line[start], 1), gc));
	}
	return i;
}

static int	handle_word(char *line, int i, t_token **tokens, t_gc *gc)
{
	int start = i;
	while (line[i] && !ft_isspace(line[i]) && !is_special_char(line[i]))
		i++;
	char *word = gc_strndup(gc, &line[start], i - start);
	add_token(tokens, new_token(TOKEN_WORD, word, gc));
	return i;
}

t_token	*tokenize(char *line, t_gc *gc)
{
	int i = 0;
	t_token *tokens = NULL;

	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (line[i] == '\'' || line[i] == '"')
			i = handle_quote(line, i, &tokens, gc);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			i = handle_operator(line, i, &tokens, gc);
		else if (line[i] == '$')
			i = handle_dollar(line, i, &tokens, gc);
		else
			i = handle_word(line, i, &tokens, gc);
	}
	return tokens;
}
