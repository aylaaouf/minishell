/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:49:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:11:29 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_INPUT || type == TOKEN_OUTPUT || type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

static char	*process_token_value(t_gc *gc, t_token *token)
{
	char	*value;
	int		len;

	value = token->value;
	len = ft_strlen(value);
	if ((token->type == TOKEN_DQUOTE || token->type == TOKEN_SQUOTE) && len >= 2
		&& value[0] == value[len - 1] && (value[0] == '"' || value[0] == '\''))
	{
		return (gc_strndup(gc, value + 1, len - 2));
	}
	return (gc_strdup(gc, value));
}

static t_token	*handle_redirection_cmd(t_gc *gc, t_command *current,
		t_token *tokens)
{
	char	*type;

	if (!tokens->next)
		return (tokens->next);
	type = get_redirection_type(tokens->type);
	tokens = tokens->next;
	add_redirection(gc, current, type, tokens->value);
	if (tokens && tokens->type == TOKEN_HEREDOC)
		current->has_heredoc = 1;
	return (tokens);
}

static t_command	*handle_pipe(t_gc *gc, t_command *current)
{
	current->next = new_command(gc);
	return (current->next);
}

t_command	*parse_tokens(t_gc *gc, t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	char		*processed_value;

	head = new_command(gc);
	current = head;
	while (tokens)
	{
		if (is_word_token(tokens->type))
		{
			processed_value = process_token_value(gc, tokens);
			add_argument(gc, current, processed_value, TOKEN_WORD);
		}
		else if (is_redirection_token(tokens->type) && tokens->next)
			tokens = handle_redirection_cmd(gc, current, tokens);
		else if (tokens->type == TOKEN_PIPE)
			current = handle_pipe(gc, current);
		tokens = tokens->next;
	}
	return (head);
}
