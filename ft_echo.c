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

char    **fill_args(char *input)
{
    char **args;

    args = ft_split(input, ' ');
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

void    ft_echo(char *input)
{
    int i;
    int j;
    char **args;

    j = 0;
    args = fill_args(input);
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