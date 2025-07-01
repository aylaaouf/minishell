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
        if (!ft_strcmp(env->key, key))
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
            }
            else
            {
                char *start = ptr;
                while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
                    ptr++;
                char *key = gc_strndup(gc, start, ptr - start);
                char *val = get_env_value_echo(key, env);
                expanded = gc_strjoin_free_a(gc, expanded, val);
            }
        }
        else
        {
            char temp[2] = {*ptr++, '\0'};
            expanded = gc_strjoin_free_a(gc, expanded, temp);
        }
    }
    return expanded;
}


static void print_expanded(t_gc *gc, char *input, t_env *env)
{
    char quote = 0;
    char *result = gc_strdup(gc, "");
    for (size_t i = 0; input[i]; )
    {
        if (!quote && (input[i] == '\'' || input[i] == '"'))
        {
            quote = input[i++];
        }
        else if (quote && input[i] == quote)
        {
            quote = 0;
            i++;
        }
        else if (!quote && input[i] == '$')
        {
            i++;
            if (input[i] == '?')
            {
                char *exit_str = ft_itoa_gc(gc, g_last_exit_status);
                result = gc_strjoin_free_a(gc, result, exit_str);
                i++;
            }
            else if (input[i] && (ft_isalpha(input[i]) || input[i] == '_'))
            {
                size_t start = i;
                while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                    i++;
                char *key = gc_strndup(gc, &input[start], i - start);
                char *val = get_env_value_echo(key, env);
                result = gc_strjoin_free_a(gc, result, val);
            }
            else if (input[i] == '\0')
            {
                result = ft_strjoin_char_gc(gc, result, '$');
            }
            else
            {
                result = ft_strjoin_char_gc(gc, result, '$');
                result = ft_strjoin_char_gc(gc, result, input[i++]);
            }
        }
        else if (quote == '\'' && input[i]) // inside single quotes, no expand
        {
            result = ft_strjoin_char_gc(gc, result, input[i++]);
        }
        else // normal char or inside double quote
        {
            result = ft_strjoin_char_gc(gc, result, input[i++]);
        }
    }
    printf("%s", result);
}
static int check_flag(char *arg)
{
    if (!arg || arg[0] != '-')
        return 0;
    for (int i = 1; arg[i]; i++)
        if (arg[i] != 'n')
            return 0;
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

    char *joined = gc_strdup(gc, "");
    while (args[i])
    {
        joined = gc_strjoin_free_a(gc, joined, args[i]);
        if (args[i + 1])
            joined = ft_strjoin_char_gc(gc, joined, ' ');
        i++;
    }

    print_expanded(gc, joined, env);

    if (newline)
        printf("\n");

    g_last_exit_status = 0;
}
