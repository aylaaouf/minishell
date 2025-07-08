/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:58:02 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 14:48:49 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*new_token(t_token_type type, char *value, t_gc *gc)
{
	t_token	*token;

	token = gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*cur;

	if (!*head)
		*head = new;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

int	handle_heredoc_quotes(char *line, int i, t_token **tokens, t_gc *gc)
{
	char			quote;
	int				start;
	char			*full_with_quotes;
	t_token_type	q_type;

	quote = line[i++];
	start = i;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
	{
		full_with_quotes = gc_strndup(gc, &line[start - 1], i - start + 2);
		if (quote == '\'')
			q_type = TOKEN_SQUOTE;
		else
			q_type = TOKEN_DQUOTE;
		add_token(tokens, new_token(q_type, full_with_quotes, gc));
		return (i + 1);
	}
	return (i);
}
