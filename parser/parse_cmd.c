/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:49:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 22:53:07 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_word_token(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_SQUOTE
		|| type == TOKEN_DQUOTE);
}

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_INPUT || type == TOKEN_OUTPUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
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

	head = new_command(gc);
	current = head;
	while (tokens)
	{
		if (is_word_token(tokens->type))
			add_argument(gc, current, tokens->value, tokens->type);
		else if (is_redirection_token(tokens->type) && tokens->next)
			tokens = handle_redirection_cmd(gc, current, tokens);
		else if (tokens->type == TOKEN_PIPE)
			current = handle_pipe(gc, current);
		tokens = tokens->next;
	}
	return (head);
}
