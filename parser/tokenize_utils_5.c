/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:04:33 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:04:45 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	init_tokenize_data(t_tokenize_context *ctx, t_token **tokens, t_gc *gc)
{
	ctx->tokens = tokens;
	ctx->gc = gc;
	ctx->is_after_heredoc = 0;
	ctx->has_space_before = true;
}

int	process_whitespace_chars(char *line, int i)
{
	while (line[i] && ft_isspace(line[i]))
		i++;
	return (i);
}

int	handle_operator_token(char *line, int i, t_tokenize_params *params,
		t_tokenize_context *ctx)
{
	if (line[i] == '<' && line[i + 1] == '<')
		ctx->is_after_heredoc = 1;
	init_tokenize_params(params, i, ctx->is_after_heredoc,
		ctx->has_space_before);
	i = handle_operator(line, params);
	ctx->has_space_before = false;
	return (i);
}

int	handle_word_quote_token(char *line, int i, t_tokenize_params *params,
		t_tokenize_context *ctx)
{
	init_tokenize_params(params, i, ctx->is_after_heredoc,
		ctx->has_space_before);
	i = handle_word_or_quotes(line, i, params);
	ctx->is_after_heredoc = 0;
	ctx->has_space_before = false;
	return (i);
}
