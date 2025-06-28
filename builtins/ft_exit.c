/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:25:50 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/28 03:36:58 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

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

char    **fill_args_exit(char *input)
{
    char **args;

    args = ft_split(input, ' ');
    return (args);
}

int is_number(char *str)
{
    int i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
            i++;
        else
            return (0);
    }
    return (1);
}

int ft_exit(char *input)
{
    char **args;
    int status;

    args = fill_args_exit(input);
    printf("exit\n");
    if (args[1] && !is_number(args[1]))
    {
        printf("minishell: exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }
    if (args[1] && args[2] != NULL)
    {
        printf("minishell: exit: too many arguments\n");
        return (1);
    }
    if (args[1])
        status = ft_atoi(args[1]);
    else
        status = 0;
    free_2d_array(args);
    g_last_exit_status = status;
    exit(status);
}