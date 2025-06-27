/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:07:32 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/07 15:12:09 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *remove_outer_quotes(t_gc *gc, char *str, char quote)
{
    size_t len = strlen(str);
    if (len >= 2 && str[0] == quote && str[len - 1] == quote)
        return (gc_strndup(gc, str + 1, len - 2));
    return (gc_strdup(gc, str));
}

void quote_management(t_gc *gc, t_token *tokens)
{
    char *cleaned;

    while (tokens)
    {
        if (tokens->type == TOKEN_SQUOTE)
        {
            cleaned = remove_outer_quotes(gc, tokens->value, '\'');
            tokens->value = cleaned;
            tokens->type = TOKEN_WORD;
        }
        else if (tokens->type == TOKEN_DQUOTE)
        {
            cleaned = remove_outer_quotes(gc, tokens->value, '"');
            tokens->value = cleaned;
            tokens->type = TOKEN_WORD;
        }
        tokens = tokens->next;
    }
}

