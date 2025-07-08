/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:07:32 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 13:28:14 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strip_quotes(t_gc *gc, const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"'
				&& str[len - 1] == '"')))
		return (gc_strndup(gc, str + 1, len - 2));
	return (gc_strdup(gc, str));
}

void	quote_management(t_gc *gc, t_token *tokens)
{
	t_token	*cur;
	t_token	*prev;

	cur = tokens;
	prev = NULL;
	while (cur)
	{
		if ((cur->type == TOKEN_SQUOTE || cur->type == TOKEN_DQUOTE) && !(prev
				&& prev->type == TOKEN_HEREDOC))
		{
			cur->value = ft_strip_quotes(gc, cur->value);
		}
		prev = cur;
		cur = cur->next;
	}
}
