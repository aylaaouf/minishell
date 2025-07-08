/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:40:46 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/11 11:02:14 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (printf("bash: syntax error near unexpected token `|'\n"), 0);
	while (current)
	{
		if (current->type == TOKEN_PIPE && (!current->next
				|| current->next->type == TOKEN_PIPE))
			return (printf("bash: syntax error near unexpected token `|'\n"), 0);
		else if ((current->type == TOKEN_OUTPUT || current->type == TOKEN_APPEND
				|| current->type == TOKEN_INPUT || current->type == TOKEN_HEREDOC)
			&& (!current->next || (current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_SQUOTE
				&& current->next->type != TOKEN_DQUOTE)))
		{
			printf("bash: syntax error near unexpected token `%s'\n",
				current->value);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

