/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:25:50 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/04 11:52:25 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_atoi(const char *str)
{
    int i;
    int result;
    int sign;

    i = 0;
    result = 0;
    sign = 1;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
        {
            sign *= -1;
        }
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + str[i] - '0';
        i++;
    }
    return (result * sign);
}

int is_number(char *str)
{
    int i;

    i = 0;
    if (!str || str[0] == '\0')
        return (0);
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
            i++;
        else if (str[i] == ' ' || str[i] == '\t')
        {
            while (str[i] == ' ' || str[i] == '\t')
                i++;
            break ;
        }
        else
            return (0);
    }
    return (str[i] == '\0');
}

int ft_exit(char **args)
{
    int status;

    printf("exit\n");
    if (args[1] && !is_number(args[1]))
    {
        write(2, "minishell: exit: ", 18);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 29);
		g_last_exit_status = 2;
		exit(2);
    }
    if (args[1] && args[2] != NULL)
    {
        write(2, "minishell: exit: too many arguments\n", 36);
        g_last_exit_status = 1;
        return (1);
    }
    if (args[1])
        status = ft_atoi(args[1]);
    else
        status = 0;
    g_last_exit_status = status;
    exit(status);
}
