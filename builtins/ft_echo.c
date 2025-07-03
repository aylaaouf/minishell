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


static int check_flag(char *arg)
{
    if (!arg || arg[0] != '-')
        return 0;
    for (int i = 1; arg[i]; i++)
        if (arg[i] != 'n')
            return 0;
    return 1;
}

void ft_echo(char **args)
{
    int i = 1;
    int newline = 1;
    
    while (args[i] && check_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
		/*if (args[i + 1])*/
		/*	printf(" ");*/
        i++;
    }
    if (newline)
        printf("\n");

    g_last_exit_status = 0;
}
