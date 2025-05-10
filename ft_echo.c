/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:43:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/09 10:46:06 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value_echo(char *var, char **env)
{
    for (int i = 0; env[i]; i++)
    {
        if (strncmp(env[i], var, strlen(var)) == 0 && env[i][strlen(var)] == '=')
            return &env[i][strlen(var) + 1];
    }
    return "";
}

char *expand_env(char *arg, char **env)
{
    if (arg[0] == '$')
    {
        char *var = arg + 1; // Skip the '$'
        char *value = get_env_value_echo(var, env);
        return value ? value : "";
    }
    return arg;
}

void ft_echo(char *input, char **env)
{
    char *token;
    int newline = 1;
    int first_word = 1;

    token = strtok(input + 5, " ");
    if (token && strcmp(token, "-n") == 0)
    {
        newline = 0; // Disable newline if -n flag is present
        token = strtok(NULL, " ");
    }

    while (token)
    {
        if (first_word == 0)
            printf(" ");
        else
            first_word = 0;

        printf("%s", expand_env(token, env));
        token = strtok(NULL, " ");
    }

    if (newline)
        printf("\n");
}
