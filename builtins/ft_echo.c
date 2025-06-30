/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:43:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/28 07:21:33 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_env_value_echo(char *key, t_env *env)
{
    while (env)
    {
        if (!strcmp(env->key, key))
            return env->value;
        env = env->next;
    }
    return "";
}

char *expand_env(t_gc *gc, char *input, t_env *env)
{
    char *expanded = gc_strdup(gc, "");
    char *ptr = input;

    while (*ptr)
    {
        if (*ptr == '$' && *(ptr + 1))
        {
            ptr++;
            if (*ptr == '?')
            {
                char *status_str = ft_itoa_gc(gc, g_last_exit_status);
                expanded = gc_strjoin_free_a(gc, expanded, status_str);
                ptr++;
                continue;
            }
            char *var_start = ptr;
            while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
                ptr++;
            char *var_name = gc_strndup(gc, var_start, ptr - var_start);
            char *value = get_env_value_echo(var_name, env);
            expanded = gc_strjoin_free_a(gc, expanded, value);
        }
        else
        {
            char temp[2] = {*ptr, '\0'};
            expanded = gc_strjoin_free_a(gc, expanded, temp);
            ptr++;
        }
    }
    return expanded;
}

int check_flag(char *input)
{
    int i;

    if (!input || input[0] != '-')
        return 0;
    i = 1;
    while (input[i])
    {
        if (input[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

void ft_echo(t_gc *gc, char **args, t_env *env)
{
    int i = 1;
    int newline = 1;

    if (args[i] && check_flag(args[i]))
    {
        newline = 0;
        i++;
    }

    char *result = gc_strdup(gc, "");

    while (args[i])
    {
        char *arg = args[i];
        char *clean = gc_strdup(gc, "");

        for (int j = 0; arg[j]; j++)
        {
            if (arg[j] == '\'')
                continue;
            else if (arg[j] == '"')
                continue;
            else
                clean = ft_strjoin_char_gc(gc, clean, arg[j]);
        }

        if (ft_strchr(arg, '"'))
        {
            char *expanded = expand_env(gc, clean, env);
            result = gc_strjoin_free_a(gc, result, expanded);
        }
        else if (ft_strchr(arg, '\''))
        {
            result = gc_strjoin_free_a(gc, result, clean);
        }
        else
        {
            char *expanded = expand_env(gc, clean, env);
            result = gc_strjoin_free_a(gc, result, expanded);
        }

        i++;
    }
    printf("%s", result);
    if (newline)
        printf("\n");

    g_last_exit_status = 0;
}

