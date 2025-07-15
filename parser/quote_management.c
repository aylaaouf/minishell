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


static int get_token_content_length(t_token *token)
{
    if (token->type == TOKEN_DQUOTE || token->type == TOKEN_SQUOTE)
    {
        int len = ft_strlen(token->value);
        if (len >= 2 && token->value[0] == token->value[len - 1] && 
            (token->value[0] == '"' || token->value[0] == '\''))
            return len - 2; // Remove quotes
        else
            return len;
    }
    return ft_strlen(token->value);
}
void quote_management(t_gc *gc, t_token *tokens)
{
    t_token *current = tokens;
    
	current = current->next;
    while (current)
    {
        if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE || current->type == TOKEN_WORD)
        {
            // Check if we need to join consecutive tokens (only if no spaces between them)
            t_token *start = current;
            char *joined_value = NULL;
            int total_len = 0;
            
            // Calculate total length needed for joining (only tokens without spaces before them)
            t_token *temp = current;
            total_len += get_token_content_length(temp); // Add first token
            temp = temp->next;
            
            // Only join if next tokens don't have spaces before them
            while (temp && (temp->type == TOKEN_DQUOTE || temp->type == TOKEN_SQUOTE || temp->type == TOKEN_WORD) 
                   && !temp->has_space_before) // KEY: Check if no space before this token
            {
                total_len += get_token_content_length(temp);
                temp = temp->next;
            }
            
            // If we only have one token to process, just remove quotes and continue
            if (temp == current->next || (current->next && current->next->has_space_before))
            {
                // Single token - just remove quotes if needed
                if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE)
                {
                    char *old_value = current->value;
                    int len = ft_strlen(old_value);
                    
                    if (len >= 2 && old_value[0] == old_value[len - 1] && 
                        (old_value[0] == '"' || old_value[0] == '\''))
                    {
                        current->value = gc_strndup(gc, old_value + 1, len - 2);
                    }
                    current->type = TOKEN_WORD;
                }
                current = current->next;
                continue;
            }
            
            // Multiple tokens to join - allocate memory for joined string
            joined_value = gc_malloc(gc, total_len + 1);
            joined_value[0] = '\0';
            
            // Join consecutive tokens that don't have spaces before them
            t_token *join_current = current;
            while (join_current && (join_current->type == TOKEN_DQUOTE || join_current->type == TOKEN_SQUOTE || join_current->type == TOKEN_WORD))
            {
                // Stop if we encounter a token with space before it (except the first one)
                if (join_current != current && join_current->has_space_before)
                    break;
                
                if (join_current->type == TOKEN_DQUOTE || join_current->type == TOKEN_SQUOTE)
                {
                    // Remove quotes and append content
                    char *old_value = join_current->value;
                    int len = ft_strlen(old_value);
                    
                    if (len >= 2 && old_value[0] == old_value[len - 1] && 
                        (old_value[0] == '"' || old_value[0] == '\''))
                    {
                        strncat(joined_value, old_value + 1, len - 2);
                    }
                    else
                    {
                        strcat(joined_value, old_value);
                    }
                }
                else
                {
                    strcat(joined_value, join_current->value);
                }
                join_current = join_current->next;
            }
            
            // Update the first token with joined value and mark as word
            start->value = joined_value;
            start->type = TOKEN_WORD;
            
            // Mark intermediate tokens for removal
            t_token *temp_remove = start->next;
            while (temp_remove && temp_remove != join_current)
            {
                temp_remove->type = TOKEN_REMOVED;
                temp_remove = temp_remove->next;
            }
            
            current = join_current; // Continue from the next unprocessed token
        }
        else
        {
            current = current->next;
        }
    }
}

// Helper function to get content length (removing quotes if needed)
