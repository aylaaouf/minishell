/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:58:02 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:09:04 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*new_token(t_token_type type, char *value, t_gc *gc,
		bool has_space_before)
{
	t_token	*token;

	token = gc_malloc(gc, sizeof(t_token));
	token->type = type;
	token->value = value;
	token->has_space_before = has_space_before;
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

int	handle_heredoc_quotes(char *line, t_heredoc_params *params)
{
	char			quote;
	int				start;
	char			*full_with_quotes;
	t_token_type	q_type;

	quote = line[params->i++];
	start = params->i;
	while (line[params->i] && line[params->i] != quote)
		params->i++;
	if (line[params->i] == quote)
	{
		full_with_quotes = gc_strndup(params->gc, &line[start - 1], params->i
				- start + 2);
		if (quote == '\'')
			q_type = TOKEN_SQUOTE;
		else
			q_type = TOKEN_DQUOTE;
		add_token(params->tokens, new_token(q_type, full_with_quotes,
				params->gc, params->has_space_before));
		return (params->i + 1);
	}
	return (params->i);
}
