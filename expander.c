/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:06:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/23 13:57:48 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return "";
}

char *extract_var_name(t_gc *gc, char *str, size_t *i)
{
    size_t start = *i;
    if (str[*i] == '?')
    {
        (*i)++;
        return gc_strndup(gc, "?", 1);
    }
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        (*i)++;
    return gc_strndup(gc, &str[start], *i - start);
}

char *expand_token_value(t_gc *gc, char *str, t_env *env)
{
    size_t i = 0;
    char *result = gc_strdup(gc, "");
    char quote = 0;

    while (str[i])
    {
        if (!quote && (str[i] == '"' || str[i] == '\''))
        {
            quote = str[i++];
            continue;
        }
        else if (quote && str[i] == quote)
        {
            quote = 0;
            i++;
            continue;
        }
        if (str[i] == '$' && quote != '\'')
        {
            i++;
            if (str[i] == '\0')
            {
                result = ft_strjoin_char_gc(gc, result, '$');
                break;
            }
			if (str[i] != '?' && !ft_isalnum(str[i]) && str[i] != '_')
            {
                result = ft_strjoin_char_gc(gc, result, '$');
                result = ft_strjoin_char_gc(gc, result, str[i]);
                i++;
                continue;
            }
            char *key = extract_var_name(gc, str, &i);
            if (key[0] == '?')
            {
                char *status_str = ft_itoa_gc(gc, g_last_exit_status);
                result = gc_strjoin_free_a(gc, result, status_str);
            }
            else
            {
                char *val = get_env_value(env, key);
                result = gc_strjoin_free_a(gc, result, val);
            }
        }
        else
        {
            result = ft_strjoin_char_gc(gc, result, str[i]);
            i++;
        }
    }
    return result;
}

void expander(t_gc *gc, t_token *tokens, t_env *env)
{
    while (tokens)
    {
        if ((tokens->type == TOKEN_WORD || tokens->type == TOKEN_DQUOTE) 
			&& strchr(tokens->value, '$'))
        {
            tokens->value = expand_token_value(gc, tokens->value, env);
        }
        tokens = tokens->next;
    }
}
