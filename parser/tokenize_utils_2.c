/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:02:40 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:17:51 by ayelasef         ###   ########.fr       */
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

int	handle_empty_single_quote(char *line, t_quote_params *params)
{
	char	*end_quote;
	char	*content;

	end_quote = ft_strchr(&line[params->i + 1], '\'');
	if (end_quote)
	{
		content = gc_strndup(params->gc, &line[params->i + 1], end_quote
				- &line[params->i + 1]);
		add_token(params->tokens, new_token(TOKEN_SQUOTE, content, params->gc,
				params->has_space_before));
		return (end_quote - line + 1);
	}
	return (params->i);
}

int	process_quote_in_word(char *line, int i, t_parse_context *ctx)
{
	t_quote_params	quote_params;

	if (check_if_standalone_quote(line, i, ctx->joined))
	{
		quote_params.tokens = ctx->tokens;
		quote_params.gc = ctx->gc;
		quote_params.has_space_before = ctx->has_space_before;
		return (handle_standalone_quotes(line, i, &quote_params));
	}
	if (line[i] == '\'')
		return (handle_single_quote(line, i + 1, ctx->joined, ctx->gc));
	else
		return (handle_double_quote(line, i + 1, ctx->joined, ctx->gc));
}

int	process_character_tokenize(char *line, int i, t_parse_context *ctx)
{
	t_heredoc_params	heredoc_params;

	if (line[i] == '"' || line[i] == '\'')
	{
		if (ctx->is_after_heredoc)
		{
			heredoc_params.i = i;
			heredoc_params.tokens = ctx->tokens;
			heredoc_params.gc = ctx->gc;
			heredoc_params.has_space_before = ctx->has_space_before;
			return (handle_heredoc_quotes(line, &heredoc_params));
		}
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

static int	handle_quoted_string(char *line, t_quote_params *params,
		char quote_char)
{
	int		start;
	int		len;
	char	*value;

	start = params->i;
	len = 0;
	params->i++;
	len++;
	while (line[params->i] && line[params->i] != quote_char)
	{
		params->i++;
		len++;
	}
	if (line[params->i] == quote_char)
	{
		params->i++;
		len++;
	}
	value = gc_strndup(params->gc, &line[start], len);
	if (quote_char == '"')
		add_token(params->tokens, new_token(TOKEN_DQUOTE, value, params->gc,
				params->has_space_before));
	else
		add_token(params->tokens, new_token(TOKEN_SQUOTE, value, params->gc,
				params->has_space_before));
	return (params->i);
}

static int	handle_word(char *line, t_word_params *params)
{
	int		start;
	char	*value;

	start = params->i;
	while (line[params->i] && !ft_isspace(line[params->i])
		&& !is_operator_char(line[params->i]) && line[params->i] != '"'
		&& line[params->i] != '\'')
	{
		params->i++;
	}
	if (params->i > start)
	{
		value = gc_strndup(params->gc, &line[start], params->i - start);
		add_token(params->tokens, new_token(TOKEN_WORD, value, params->gc,
				params->has_space_before));
	}
	return (params->i);
}

int	handle_word_or_quotes(char *line, int i, t_tokenize_params *params)
{
	t_quote_params	quote_params;
	t_word_params	word_params;

	if (line[i] == '"')
	{
		quote_params.tokens = params->tokens;
		quote_params.gc = params->gc;
		quote_params.i = i;
		quote_params.has_space_before = params->has_space_before;
		return (handle_quoted_string(line, &quote_params, '"'));
	}
	else if (line[i] == '\'')
	{
		quote_params.tokens = params->tokens;
		quote_params.gc = params->gc;
		quote_params.i = i;
		quote_params.has_space_before = params->has_space_before;
		return (handle_quoted_string(line, &quote_params, '\''));
	}
	else
	{
		word_params.tokens = params->tokens;
		word_params.gc = params->gc;
		word_params.i = i;
		word_params.has_space_before = params->has_space_before;
		return (handle_word(line, &word_params));
	}
}
