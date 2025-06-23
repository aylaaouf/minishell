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

char *extract_var_name(char *str, size_t *i)
{
    size_t start = *i;
    while (str[*i] && (str[*i] == '_' || 
                      (str[*i] >= 'A' && str[*i] <= 'Z') ||
                      (str[*i] >= 'a' && str[*i] <= 'z') ||
                      (str[*i] >= '0' && str[*i] <= '9')))
    {
        (*i)++;
    }
    return ft_strndup(&str[start], *i - start);
}

char *expand_token_value(char *str, t_env *env, int last_exit_status)
{
    size_t i = 0;
    char *result = ft_calloc(1, 1);

    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (str[i] == '?')
            {
                char *status_str = ft_itoa(last_exit_status);
                result = ft_strjoin_free(result, status_str);
                free(status_str);
                i++;
            }
            else if (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
            {
                char *var_name = extract_var_name(str, &i);
                char *var_value = get_env_value(env, var_name);
                result = ft_strjoin_free(result, var_value ? var_value : "");
                free(var_name);
            }
            else
            {
                result = ft_strjoin_char(result, '$');
            }
        }
        else
        {
            result = ft_strjoin_char(result, str[i]);
            i++;
        }
    }
    return result;
}

void expander(t_token *tokens, t_env *env, int last_exit_status)
{
    char *expanded;

    while (tokens)
    {
        if ((tokens->type == TOKEN_WORD || tokens->type == TOKEN_DQUOTE) &&
            strchr(tokens->value, '$'))
        {
            expanded = expand_token_value(tokens->value, env, last_exit_status);
            free(tokens->value);
            tokens->value = expanded;
        }
        tokens = tokens->next;
    }
}

