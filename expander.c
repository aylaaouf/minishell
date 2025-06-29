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
    return NULL;
}

char *extract_var_name(t_gc *gc, char *str, size_t *i)
{
    size_t start = *i;
    while (str[*i] && (str[*i] == '_' ||
                      (str[*i] >= 'A' && str[*i] <= 'Z') ||
                      (str[*i] >= 'a' && str[*i] <= 'z') ||
                      (str[*i] >= '0' && str[*i] <= '9')))
    {
        (*i)++;
    }
    return gc_strndup(gc, &str[start], *i - start);
}

char *expand_token_value(t_gc *gc, char *str, t_env *env)
{
    size_t i = 0;
    char *result = gc_strdup(gc, "");

    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (str[i] == '?')
            {
                char *status_str = ft_itoa_gc(gc, g_last_exit_status);
                result = gc_strjoin_free_a(gc, result, status_str);
                i++;
            }
            else if (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
            {
                char *var_name = extract_var_name(gc, str, &i);
                char *var_value = get_env_value(env, var_name);
                result = gc_strjoin_free_a(gc, result, var_value ? var_value : "");
            }
            else
                result = ft_strjoin_char_gc(gc, result, '$');
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
        if ((tokens->type == TOKEN_WORD || tokens->type == TOKEN_DQUOTE) &&
            strchr(tokens->value, '$'))
        {
            tokens->value = expand_token_value(gc, tokens->value, env);
        }
        tokens = tokens->next;
    }
}
