/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:34:45 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 16:03:10 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirect(char *line, int i, t_token **tokens, t_gc *gc)
{
	if (line[i + 1] == '<')
	{
		add_token(tokens, new_token(TOKEN_HEREDOC, gc_strndup(gc, "<<", 2),
				gc));
		return (i + 2);
	}
	else
	{
		add_token(tokens, new_token(TOKEN_INPUT, gc_strndup(gc, "<", 1), gc));
		return (i + 1);
	}
}

static int	handle_output_redirect(char *line, int i, t_token **tokens,
		t_gc *gc)
{
	if (line[i + 1] == '>')
	{
		add_token(tokens, new_token(TOKEN_APPEND, gc_strndup(gc, ">>", 2), gc));
		return (i + 2);
	}
	else
	{
		add_token(tokens, new_token(TOKEN_OUTPUT, gc_strndup(gc, ">", 1), gc));
		return (i + 1);
	}
}

static int	handle_operator(char *line, int i, t_token **tokens, t_gc *gc)
{
	if (line[i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, gc_strndup(gc, "|", 1), gc));
		return (i + 1);
	}
	else if (line[i] == '<')
		return (handle_input_redirect(line, i, tokens, gc));
	else if (line[i] == '>')
		return (handle_output_redirect(line, i, tokens, gc));
	return (i + 1);
}

static void	init_tokenize_params(t_tokenize_params *params, t_token **tokens,
		t_gc *gc, int is_after_heredoc)
{
	params->tokens = tokens;
	params->gc = gc;
	params->is_after_heredoc = is_after_heredoc;
}

t_token	*tokenize(char *line, t_gc *gc)
{
	t_token				*tokens;
	t_tokenize_params	params;

	int (i), is_after_heredoc;
	i = 0;
	is_after_heredoc = 0;
	tokens = NULL;
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
			init_tokenize_params(&params, &tokens, gc, is_after_heredoc);
			i = handle_word_or_quotes(line, i, &params);
			is_after_heredoc = 0;
		}
	}
	return (tokens);
}
