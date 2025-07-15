/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:02:40 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 22:53:17 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_dollar_sign(char *line, int i, char **joined, t_gc *gc)
{
	i++;
	if (line[i] == '?')
		return (handle_dollar_question(line, i - 1, joined, gc));
	else
		return (handle_dollar_variable(line, i, joined, gc));
}

int	handle_empty_single_quote(char *line, int i, t_token **tokens, t_gc *gc,
		bool has_space_before)
{
	char	*end_quote;
	char	*content;

	end_quote = ft_strchr(&line[i + 1], '\'');
	if (end_quote)
	{
		content = gc_strndup(gc, &line[i + 1], end_quote - &line[i + 1]);
		add_token(tokens, new_token(TOKEN_SQUOTE, content, gc,
				has_space_before));
		return (end_quote - line + 1);
	}
	return (i);
}

int	process_quote_in_word(char *line, int i, t_parse_context *ctx)
{
	if (check_if_standalone_quote(line, i, ctx->joined))
		return (handle_standalone_quotes(line, i, ctx->tokens, ctx->gc,
				ctx->has_space_before));
	if (line[i] == '\'')
		return (handle_single_quote(line, i + 1, ctx->joined, ctx->gc));
	else
		return (handle_double_quote(line, i + 1, ctx->joined, ctx->gc));
}

int	process_character_tokenize(char *line, int i, t_parse_context *ctx)
{
	if (line[i] == '"' || line[i] == '\'')
	{
		if (ctx->is_after_heredoc)
			return (handle_heredoc_quotes(line, i, ctx->tokens, ctx->gc,
					ctx->has_space_before));
		return (process_quote_in_word(line, i, ctx));
	}
	else if (line[i] == '$')
		return (handle_dollar_sign(line, i, ctx->joined, ctx->gc));
	else
	{
		*(ctx->joined) = ft_strjoin_char_gc(ctx->gc, *(ctx->joined), line[i]);
		return (i + 1);
	}
}

static int	handle_quoted_string(char *line, int i, t_token **tokens, t_gc *gc,
		char quote_char, bool has_space_before)
{
	int		start;
	int		len;
	char	*value;

	start = i;
	len = 0;
	i++;
	len++;
	while (line[i] && line[i] != quote_char)
	{
		i++;
		len++;
	}
	if (line[i] == quote_char)
	{
		i++;
		len++;
	}
	value = gc_strndup(gc, &line[start], len);
	if (quote_char == '"')
		add_token(tokens, new_token(TOKEN_DQUOTE, value, gc, has_space_before));
	else
		add_token(tokens, new_token(TOKEN_SQUOTE, value, gc, has_space_before));
	return (i);
}

static int	handle_word(char *line, int i, t_token **tokens, t_gc *gc,
		bool has_space_before)
{
	int		start;
	char	*value;

	start = i;
	while (line[i] && !ft_isspace(line[i]) && !is_operator_char(line[i])
		&& line[i] != '"' && line[i] != '\'')
	{
		i++;
	}
	if (i > start)
	{
		value = gc_strndup(gc, &line[start], i - start);
		add_token(tokens, new_token(TOKEN_WORD, value, gc, has_space_before));
	}
	return (i);
}

int	handle_word_or_quotes(char *line, int i, t_tokenize_params *params)
{
	if (line[i] == '"')
	{
		return (handle_quoted_string(line, i, params->tokens, params->gc, '"',
				params->has_space_before));
	}
	else if (line[i] == '\'')
	{
		return (handle_quoted_string(line, i, params->tokens, params->gc, '\'',
				params->has_space_before));
	}
	else
	{
		return (handle_word(line, i, params->tokens, params->gc,
				params->has_space_before));
	}
}
