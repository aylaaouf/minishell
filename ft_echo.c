/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:43:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/18 11:53:30 by ayelasef         ###   ########.fr       */
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
    return ("");
}

char *expand_env(char *input, t_env *env)
{
    char *expanded = ft_strdup("");
    char *ptr = input;
    while (*ptr)
    {
        if (*ptr == '$' && *(ptr + 1))
        {
            ptr++;
            char *var_start = ptr;
            while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
                ptr++;
            char *var_name = ft_strndup(var_start, ptr - var_start);
            char *value = get_env_value_echo(var_name, env);
            expanded = ft_strjoin(expanded, value);
            free(var_name);
        }
        else
        {
            char temp[2] = {*ptr, '\0'};
            expanded = ft_strjoin(expanded, temp);
            ptr++;
        }
    }
    return (expanded);
}

char *parse_echo_arguments(char *input, t_env *env)
{
    char *result = ft_strdup("");
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
            char *segment = ft_strndup(&input[start], i - start);
            if (quote_char == '"')
            {
                char *expanded = expand_env(segment, env);
                free(segment);
                segment = expanded;
            }
            char *tmp = ft_strjoin(result, segment);
            free(result);
            free(segment);
            result = tmp;
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
                char *digits = ft_strndup(&input[start], digit_len);
                char *tmp = ft_strjoin(result, digits);
                free(result);
                free(digits);
                result = tmp;
            }
            else
            {
                while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                    i++;
                char *var_name = ft_strndup(&input[start], i - start);
                char *value = get_env_value_echo(var_name, env);
                char *tmp = ft_strjoin(result, value);
                free(result);
                free(var_name);
                result = tmp;
            }
        }
        else
        {
            start = i;
            while (input[i] && input[i] != '\'' && input[i] != '"' && input[i] != '$')
                i++;
            char *segment = ft_strndup(&input[start], i - start);
            char *tmp = ft_strjoin(result, segment);
            free(result);
            free(segment);
            result = tmp;
        }
    }
    return (result);
}

char **fill_args(char *input, t_env *env)
{
    char **args;
    char *parsed_input = parse_echo_arguments(input, env);
    args = ft_split(parsed_input, ' ');
    free(parsed_input);
    return (args);
}

int check_flag(char *input)
{
    int i;

    if (!input || input[0] != '-')
        return (0);
    i = 1;
    while (input[i])
    {
        if (input[i] == '-' || input[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

void    ft_echo(char *input, t_env *env)
{
    int i;
    int j;
    char **args;

    j = 0;
    args = fill_args(input, env);
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
}
