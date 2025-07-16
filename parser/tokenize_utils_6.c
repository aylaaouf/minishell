/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:12:28 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/16 15:13:50 by ayelasef         ###   ########.fr       */
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
