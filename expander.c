/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:06:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/07 16:39:14 by ayelasef         ###   ########.fr       */
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
    return (NULL);
}

char *extract_var_name(char *str, size_t *i)
{
    size_t start = *i;
    while (str[*i] && (str[*i] == '_' || 
                       (str[*i] >= 'A' && str[*i] <= 'Z') ||
                       (str[*i] >= 'a' && str[*i] <= 'z') ||
                       (str[*i] >= '0' && str[*i] <= '9')))
        (*i)++;
    return (ft_strndup(&str[start], *i - start));
}

char *expand_token_value(char *str, t_env *env)
{
    size_t i = 0;
    char *result = ft_calloc(1, 1);

    while (str[i])
    {
        if (str[i] == '$')
        {
            if (str[i + 1] == '?')
            {
                char *exit_status = ft_itoa(g_exit_status);
                result = ft_realloc(result, ft_strlen(result) + ft_strlen(exit_status) + 1);
                ft_strcat(result, exit_status);
                free(exit_status);
                i += 2;
                continue;
            }
            else if (str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '$')
            {
                i++;
                char *var_name = extract_var_name(str, &i);
                char *var_value = get_env_value(env, var_name);
                if (var_value)
                {
                    result = ft_realloc(result, ft_strlen(result) + ft_strlen(var_value) + 1);
                    ft_strcat(result, var_value);
                }
                free(var_name);
                continue;
            }
        }

        size_t len = ft_strlen(result);
        result = ft_realloc(result, len + 2);
        result[len] = str[i];
        result[len + 1] = '\0';
        i++;
    }
    return (result);
}

void expander(t_token *tokens, t_env *env)
{
    char *expanded;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD && strchr(tokens->value, '$'))
        {
            expanded = expand_token_value(tokens->value, env);
            free(tokens->value);
            tokens->value = expanded;
        }
        tokens = tokens->next;
    }
}
