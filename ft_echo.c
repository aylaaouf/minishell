/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:43:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/28 18:56:27 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char *expand_env(t_gc *gc, char *input, t_env *env, int last_exit_status)
{
    char *expanded = gc_strdup(gc, "");
    char *ptr = input;

    while (*ptr)
    {
        if (*ptr == '$' && *(ptr + 1))
        {
            ptr++;
            if (*ptr == '?') {
                char status_str[12];
                snprintf(status_str, sizeof(status_str), "%d", last_exit_status);
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

char *parse_echo_arguments(t_gc *gc, char *input, t_env *env, int last_exit_status)
{
    char *result = gc_strdup(gc, "");
    size_t i = 0;
    size_t start;
    char quote_char = '\0';

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            quote_char = input[i++];
            start = i;
            while (input[i] && input[i] != quote_char)
                i++;
            char *segment = gc_strndup(gc, &input[start], i - start);
            if (quote_char == '"')
                segment = expand_env(gc, segment, env, last_exit_status);
            result = gc_strjoin_free_a(gc, result, segment);
            if (input[i])
                i++;
        }
        else if (input[i] == '$' && input[i + 1])
        {
            i++;
            start = i;
            size_t digit_len = 0;
            while (input[i] && isdigit(input[i]))
            {
                i++;
                digit_len++;
            }

            if (digit_len > 0)
            {
                char *digits = gc_strndup(gc, &input[start], digit_len);
                result = gc_strjoin_free_a(gc, result, digits);
            }
            else
            {
                while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                    i++;
                char *var_name = gc_strndup(gc, &input[start], i - start);
                char *value = get_env_value_echo(var_name, env);
                result = gc_strjoin_free_a(gc, result, value);
            }
        }
        else
        {
            start = i;
            while (input[i] && input[i] != '\'' && input[i] != '"' && input[i] != '$')
                i++;
            char *segment = gc_strndup(gc, &input[start], i - start);
            result = gc_strjoin_free_a(gc, result, segment);
        }
    }
    return result;
}

char **fill_args(t_gc *gc, char *input, t_env *env, int last_exit_status)
{
    char *parsed_input = parse_echo_arguments(gc, input, env, last_exit_status);
    return ft_split(parsed_input, ' ');
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

void ft_echo(t_gc *gc, char *input, t_env *env, int last_exit_status)
{
    int i;
    char **args;

    args = fill_args(gc, input, env, last_exit_status);
    if (args[1] && !check_flag(args[1]))
    {
        i = 1;
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        i = 2;
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
            i++;
        }
    }
    free_2d_array(args);
}

