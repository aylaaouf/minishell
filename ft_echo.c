/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:43:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/12 19:37:15 by aylaaouf         ###   ########.fr       */
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
    return expanded;
}

char *process_quotes(char *arg, t_env *env)
{
    if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
        return (ft_strndup(arg + 1, ft_strlen(arg) - 2));
    else if (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"')
    {
        char *trimmed = ft_strndup(arg + 1, ft_strlen(arg) - 2);
        return expand_env(trimmed, env);
    }
    return (expand_env(arg, env));
}


char **fill_args(char *input, t_env *env)
{
	int	i;

    char **args = ft_split(input, ' ');
	i = 0;
    while (args[i])
	{
        args[i] = process_quotes(args[i], env);
		i++;
	}
    return args;
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
