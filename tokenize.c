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

static bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token	*new_token(t_token_type type, char *value, t_gc *gc)
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

static void	add_token(t_token **head, t_token *new)
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

static int	handle_word_or_quotes(char *line, int i, t_token **tokens, t_gc *gc, int is_after_heredoc)
{
	char			*joined;
	int				start_i;
	t_token_type	final_type;
	char			quote;
	int				start;
	char			*quoted;
	int				var_start;
	char			*var;
	char			*full;

	joined = gc_strdup(gc, "");
	start_i = i;
	final_type = TOKEN_WORD;
	while (line[i] && !ft_isspace(line[i]) && !is_operator_char(line[i]))
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			start = i;
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == quote)
			{
				if (is_after_heredoc)
				{
					char *full_with_quotes = gc_strndup(gc, &line[start - 1], i - start + 2);
					t_token_type q_type = (quote == '\'') ? TOKEN_SQUOTE : TOKEN_DQUOTE;
					add_token(tokens, new_token(q_type, full_with_quotes, gc));
					return (i + 1);
				}
				quoted = gc_strndup(gc, &line[start], i - start);
				if (quote == '\'')
				{
					if (ft_strlen(joined) == 0 && !line[i + 1]
						&& start_i == start - 1)
					{
						add_token(tokens, new_token(TOKEN_SQUOTE, quoted, gc));
						return (i + 1);
					}
					final_type = TOKEN_WORD;
				}
				joined = gc_strjoin_free_a(gc, joined, quoted);
				i++;
			}
			else
			{
				joined = ft_strjoin_char_gc(gc, joined, line[start - 1]);
			}
		}
		else if (line[i] == '$')
		{
			start = i++;
			if (line[i] == '?')
			{
				joined = gc_strjoin_free_a(gc, joined, gc_strndup(gc, &line[start], 2));
				i++;
			}
			else
			{
				var_start = i;
				while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
					i++;
				if (i > var_start)
				{
					var = gc_strndup(gc, &line[var_start], i - var_start);
					full = gc_strjoin_free_a(gc, gc_strdup(gc, "$"), var);
					joined = gc_strjoin_free_a(gc, joined, full);
				}
				else
				{
					joined = ft_strjoin_char_gc(gc, joined, '$');
				}
			}
		}
		else
		{
			joined = ft_strjoin_char_gc(gc, joined, line[i]);
			i++;
		}
	}
	add_token(tokens, new_token(final_type, joined, gc));
	return (i);
}

static int	handle_operator(char *line, int i, t_token **tokens, t_gc *gc)
{
	if (line[i] == '|')
		add_token(tokens, new_token(TOKEN_PIPE, gc_strndup(gc, "|", 1), gc));
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(tokens, new_token(TOKEN_HEREDOC, gc_strndup(gc, "<<", 2),
					gc));
			return (i + 2);
		}
		else
			add_token(tokens, new_token(TOKEN_INPUT, gc_strndup(gc, "<", 1),
					gc));
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(tokens, new_token(TOKEN_APPEND, gc_strndup(gc, ">>", 2),
					gc));
			return (i + 2);
		}
		else
			add_token(tokens, new_token(TOKEN_OUTPUT, gc_strndup(gc, ">", 1),
					gc));
	}
	return (i + 1);
}

t_token	*tokenize(char *line, t_gc *gc)
{
	int		i;
	t_token	*tokens;
	int		is_after_heredoc;

	i = 0;
	tokens = NULL;
	is_after_heredoc = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (is_operator_char(line[i]))
		{
			if (line[i] == '<' && line[i + 1] == '<')
				is_after_heredoc = 1;
			i = handle_operator(line, i, &tokens, gc);
		}
		else
		{
			i = handle_word_or_quotes(line, i, &tokens, gc, is_after_heredoc);
			is_after_heredoc = 0;
		}
	}
	return (tokens);
}
