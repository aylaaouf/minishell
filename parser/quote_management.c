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

void quote_management(t_gc *gc, t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_DQUOTE || tokens->type == TOKEN_SQUOTE)
        {
            char *old_value = tokens->value;
            int len = ft_strlen(old_value);
            
            if (len >= 2 && old_value[0] == old_value[len - 1] && 
                (old_value[0] == '"' || old_value[0] == '\''))
                tokens->value = gc_strndup(gc, old_value + 1, len - 2);
            tokens->type = TOKEN_WORD;
        }
        tokens = tokens->next;
    }
}
