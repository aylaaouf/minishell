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
    int i = 1;

    if (!input || input[0] != '-')
        return 0;
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

    if (args[1] && check_flag(args[1]))
    {
        newline = 0;
        i++;
    }

    while (args[i])
    {
        int j = 0;
        char quote = 0;
        char *arg = args[i];
        char *result = gc_strdup(gc, "");

        while (arg[j])
        {
            if (quote == 0 && (arg[j] == '\'' || arg[j] == '"'))
            {
                quote = arg[j];
                j++;
                continue;
            }
            if (quote && arg[j] == quote)
            {
                quote = 0;
                j++;
                continue;
            }
            if (quote == '\'')
            {
                result = ft_strjoin_char_gc(gc, result, arg[j]);
                j++;
            }
            else
            {
                if (arg[j] == '$')
                {
                    j++;
                    if (arg[j] == '?')
                    {
                        char *status_str = ft_itoa_gc(gc, g_last_exit_status);
                        result = gc_strjoin_free_a(gc, result, status_str);
                        j++;
                    }
                    else if (ft_isalpha(arg[j]) || arg[j] == '_')
                    {
                        size_t var_start = j;
                        while (ft_isalnum(arg[j]) || arg[j] == '_')
                            j++;
                        char *var_name = gc_strndup(gc, &arg[var_start], j - var_start);
                        char *val = get_env_value_echo(var_name, env);
                        result = gc_strjoin_free_a(gc, result, val);
                    }
                    else
                    {
                        result = ft_strjoin_char_gc(gc, result, '$');
                    }
                }
                else
                {
                    result = ft_strjoin_char_gc(gc, result, arg[j]);
                    j++;
                }
            }
        }
        printf("%s", result);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    g_last_exit_status = 0;
}

