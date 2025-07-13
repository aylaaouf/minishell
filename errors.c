/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:40:46 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/13 00:34:22 by aylaaouf         ###   ########.fr       */
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
			return (printf("bash: syntax error near unexpected token `|'\n"),
				0);
		else if ((current->type == TOKEN_OUTPUT || current->type == TOKEN_APPEND
				|| current->type == TOKEN_INPUT
				|| current->type == TOKEN_HEREDOC) && (!current->next
				|| (current->next->type != TOKEN_WORD
					&& current->next->type != TOKEN_SQUOTE
					&& current->next->type != TOKEN_DQUOTE)))
			return (printf("bash: syntax error near unexpected token `%s'\n",
					current->value), 0);
		current = current->next;
	}
	return (1);
}

void	is_not_found(char *cmnd)
{
	if (ft_strchr(cmnd, '/'))
	{
		write(2, "minishell: ", 11);
		write(2, cmnd, ft_strlen(cmnd));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		write(2, "minishell: ", 11);
		write(2, cmnd, ft_strlen(cmnd));
		write(2, ": command not found\n", 20);
	}
}
