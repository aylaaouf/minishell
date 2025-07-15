/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:42:48 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 14:48:22 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_token_content_length(t_token *token)
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

int	calculate_total_length(t_token *current)
{
	int		total_len;
	t_token	*temp;

	total_len = 0;
	temp = current;
	total_len += get_token_content_length(temp);
	temp = temp->next;
	while (temp && (temp->type == TOKEN_DQUOTE || temp->type == TOKEN_SQUOTE
			|| temp->type == TOKEN_WORD) && !temp->has_space_before)
	{
		total_len += get_token_content_length(temp);
		temp = temp->next;
	}
	return (total_len);
}

void	remove_quotes_from_token(t_gc *gc, t_token *token)
{
	char	*old_value;
	int		len;

	old_value = token->value;
	len = ft_strlen(old_value);
	if (len >= 2 && old_value[0] == old_value[len - 1]
		&& (old_value[0] == '"' || old_value[0] == '\''))
	{
		token->value = gc_strndup(gc, old_value + 1, len - 2);
	}
	token->type = TOKEN_WORD;
}

void	append_token_content(char *joined_value, t_token *token)
{
	char	*old_value;
	int		len;

	if (token->type == TOKEN_DQUOTE || token->type == TOKEN_SQUOTE)
	{
		old_value = token->value;
		len = ft_strlen(old_value);
		if (len >= 2 && old_value[0] == old_value[len - 1]
			&& (old_value[0] == '"' || old_value[0] == '\''))
			ft_strncat(joined_value, old_value + 1, len - 2);
		else
			ft_strcat(joined_value, old_value);
	}
	else
	{
		ft_strcat(joined_value, token->value);
	}
}

char	*create_joined_value(t_gc *gc, t_token *current, int total_len)
{
	char	*joined_value;
	t_token	*join_current;

	joined_value = gc_malloc(gc, total_len + 1);
	joined_value[0] = '\0';
	join_current = current;
	while (join_current && (join_current->type == TOKEN_DQUOTE
			|| join_current->type == TOKEN_SQUOTE
			|| join_current->type == TOKEN_WORD))
	{
		if (join_current != current && join_current->has_space_before)
			break ;
		append_token_content(joined_value, join_current);
		join_current = join_current->next;
	}
	return (joined_value);
}
