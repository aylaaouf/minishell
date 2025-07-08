/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:02:40 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 16:02:26 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_sign(char *line, int i, char **joined, t_gc *gc)
{
	i++;
	if (line[i] == '?')
		return (handle_dollar_question(line, i - 1, joined, gc));
	else
		return (handle_dollar_variable(line, i, joined, gc));
}

int	handle_empty_single_quote(char *line, int i, t_token **tokens, t_gc *gc)
{
	char	*end_quote;
	char	*content;

	end_quote = ft_strchr(&line[i + 1], '\'');
	if (end_quote)
	{
		content = gc_strndup(gc, &line[i + 1], end_quote - &line[i + 1]);
		add_token(tokens, new_token(TOKEN_SQUOTE, content, gc));
		return (end_quote - line + 1);
	}
	return (i);
}

int	process_quote_in_word(char *line, int i, t_parse_context *ctx)
{
	if (check_if_standalone_quote(line, i, ctx->joined))
		return (handle_standalone_quotes(line, i, ctx->tokens, ctx->gc));
	if (line[i] == '\'')
		return (handle_single_quote(line, i + 1, ctx->joined, ctx->gc));
	else
		return (handle_double_quote(line, i + 1, ctx->joined, ctx->gc));
}

int	process_character(char *line, int i, t_parse_context *ctx)
{
	if (line[i] == '"' || line[i] == '\'')
	{
		if (ctx->is_after_heredoc)
			return (handle_heredoc_quotes(line, i, ctx->tokens, ctx->gc));
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

int	handle_word_or_quotes(char *line, int i, t_tokenize_params *params)
{
	char			*joined;
	t_token_type	final_type;
	t_parse_context	ctx;
	int				start_i;

	joined = gc_strdup(params->gc, "");
	final_type = TOKEN_WORD;
	start_i = i;
	ctx.joined = &joined;
	ctx.tokens = params->tokens;
	ctx.gc = params->gc;
	ctx.is_after_heredoc = params->is_after_heredoc;
	while (line[i] && !ft_isspace(line[i]) && !is_operator_char(line[i]))
		i = process_character(line, i, &ctx);
	if (ft_strlen(joined) > 0)
		add_token(params->tokens, new_token(final_type, joined, params->gc));
	return (i);
}
