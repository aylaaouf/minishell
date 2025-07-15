/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:07:32 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 14:44:23 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*mark_tokens_for_removal(t_token *start, t_token *end)
{
	t_token	*temp_remove;

	temp_remove = start->next;
	while (temp_remove && temp_remove != end)
	{
		temp_remove->type = TOKEN_REMOVED;
		temp_remove = temp_remove->next;
	}
	return (end);
}

static t_token	*process_single_token(t_gc *gc, t_token *current)
{
	if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE)
		remove_quotes_from_token(gc, current);
	return (current->next);
}

static t_token	*find_end_of_sequence(t_token *current)
{
	t_token	*temp;

	temp = current->next;
	while (temp && (temp->type == TOKEN_DQUOTE || temp->type == TOKEN_SQUOTE
			|| temp->type == TOKEN_WORD) && !temp->has_space_before)
	{
		temp = temp->next;
	}
	return (temp);
}

static t_token	*process_token_sequence(t_gc *gc, t_token *current)
{
	t_token	*end;
	int		total_len;
	char	*joined_value;

	end = find_end_of_sequence(current);
	if (end == current->next || (current->next
			&& current->next->has_space_before))
		return (process_single_token(gc, current));
	total_len = calculate_total_length(current);
	joined_value = create_joined_value(gc, current, total_len);
	current->value = joined_value;
	current->type = TOKEN_WORD;
	return (mark_tokens_for_removal(current, end));
}

void	quote_management(t_gc *gc, t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return ;
	current = tokens->next;
	while (current)
	{
		if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE
			|| current->type == TOKEN_WORD)
		{
			current = process_token_sequence(gc, current);
		}
		else
		{
			current = current->next;
		}
	}
}
