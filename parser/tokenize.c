/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:34:45 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:01:41 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redirect(char *line, t_tokenize_params *params)
{
	if (line[params->i + 1] == '<')
	{
		add_token(params->tokens, new_token(TOKEN_HEREDOC,
				gc_strndup(params->gc, "<<", 2), params->gc,
				params->has_space_before));
		return (params->i + 2);
	}
	else
	{
		add_token(params->tokens, new_token(TOKEN_INPUT, gc_strndup(params->gc,
					"<", 1), params->gc, params->has_space_before));
		return (params->i + 1);
	}
}

static int	handle_output_redirect(char *line, t_tokenize_params *params)
{
	if (line[params->i + 1] == '>')
	{
		add_token(params->tokens, new_token(TOKEN_APPEND, gc_strndup(params->gc,
					">>", 2), params->gc, params->has_space_before));
		return (params->i + 2);
	}
	else
	{
		add_token(params->tokens, new_token(TOKEN_OUTPUT, gc_strndup(params->gc,
					">", 1), params->gc, params->has_space_before));
		return (params->i + 1);
	}
}

void	init_tokenize_params(t_tokenize_params *params, int i,
		int is_after_heredoc, bool has_space_before)
{
	params->i = i;
	params->is_after_heredoc = is_after_heredoc;
	params->has_space_before = has_space_before;
}

int	handle_operator(char *line, t_tokenize_params *params)
{
	if (line[params->i] == '|')
	{
		add_token(params->tokens, new_token(TOKEN_PIPE, gc_strndup(params->gc,
					"|", 1), params->gc, params->has_space_before));
		return (params->i + 1);
	}
	else if (line[params->i] == '<')
		return (handle_input_redirect(line, params));
	else if (line[params->i] == '>')
		return (handle_output_redirect(line, params));
	return (params->i + 1);
}

t_token	*tokenize(char *line, t_gc *gc)
{
	t_token				*tokens;
	t_tokenize_params	params;
	t_tokenize_context	ctx;
	int					i;

	i = 0;
	tokens = NULL;
	params.tokens = &tokens;
	params.gc = gc;
	init_tokenize_data(&ctx, &tokens, gc);
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			i = process_whitespace_chars(line, i);
			ctx.has_space_before = true;
		}
		else if (is_operator_char(line[i]))
			i = handle_operator_token(line, i, &params, &ctx);
		else
			i = handle_word_quote_token(line, i, &params, &ctx);
	}
	return (tokens);
}
