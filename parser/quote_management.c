/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:07:32 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 22:53:10 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

static int	get_token_content_length(t_token *token)
{
	int	len;

	if (token->type == TOKEN_DQUOTE || token->type == TOKEN_SQUOTE)
	{
		len = ft_strlen(token->value);
		if (len >= 2 && token->value[0] == token->value[len - 1]
			&& (token->value[0] == '"' || token->value[0] == '\''))
			return (len - 2);
		else
			return (len);
	}
	return (ft_strlen(token->value));
}
void	quote_management(t_gc *gc, t_token *tokens)
{
	t_token	*current;
	t_token	*start;
	char	*joined_value;
	int		total_len;
	t_token	*temp;
	char	*old_value;
	int		len;
	t_token	*join_current;
	t_token	*temp_remove;

	current = tokens;
	current = current->next;
	while (current)
	{
		if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE
			|| current->type == TOKEN_WORD)
		{
			start = current;
			joined_value = NULL;
			total_len = 0;
			temp = current;
			total_len += get_token_content_length(temp);
			temp = temp->next;
			while (temp && (temp->type == TOKEN_DQUOTE
					|| temp->type == TOKEN_SQUOTE || temp->type == TOKEN_WORD)
				&& !temp->has_space_before)
			{
				total_len += get_token_content_length(temp);
				temp = temp->next;
			}
			if (temp == current->next || (current->next
					&& current->next->has_space_before))
			{
				if (current->type == TOKEN_DQUOTE
					|| current->type == TOKEN_SQUOTE)
				{
					old_value = current->value;
					len = ft_strlen(old_value);
					if (len >= 2 && old_value[0] == old_value[len - 1]
						&& (old_value[0] == '"' || old_value[0] == '\''))
					{
						current->value = gc_strndup(gc, old_value + 1, len - 2);
					}
					current->type = TOKEN_WORD;
				}
				current = current->next;
				continue ;
			}
			joined_value = gc_malloc(gc, total_len + 1);
			joined_value[0] = '\0';
			join_current = current;
			while (join_current && (join_current->type == TOKEN_DQUOTE
					|| join_current->type == TOKEN_SQUOTE
					|| join_current->type == TOKEN_WORD))
			{
				if (join_current != current && join_current->has_space_before)
					break ;
				if (join_current->type == TOKEN_DQUOTE
					|| join_current->type == TOKEN_SQUOTE)
				{
					old_value = join_current->value;
					len = ft_strlen(old_value);
					if (len >= 2 && old_value[0] == old_value[len - 1]
						&& (old_value[0] == '"' || old_value[0] == '\''))
						ft_strncat(joined_value, old_value + 1, len - 2);
					else
						ft_strcat(joined_value, old_value);
				}
				else
				{
					ft_strcat(joined_value, join_current->value);
				}
				join_current = join_current->next;
			}
			start->value = joined_value;
			start->type = TOKEN_WORD;
			temp_remove = start->next;
			while (temp_remove && temp_remove != join_current)
			{
				temp_remove->type = TOKEN_REMOVED;
				temp_remove = temp_remove->next;
			}
			current = join_current;
		}
		else
		{
			current = current->next;
		}
	}
}
