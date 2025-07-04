/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:48:40 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/04 15:09:09 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strjoin_env(char *s1, char *s2)
{
    size_t i;
    size_t j;
    size_t len1;
    size_t len2;
    char *array;

    if (!s1)
        return (NULL);
    if (!s2)
        return (s1);
    i = 0;
    j = 0;
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    array = malloc(len1 + len2 + 2);
    if (!array)
        return (NULL);
    while (i < ft_strlen(s1))
    {
        array[i] = s1[i];
        i++;
    }
    array[i++] = '=';
    while (j < ft_strlen(s2))
    {
        array[i] = s2[j];
        i++;
        j++;
    }
    array[i] = '\0';
    return (array);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*arr;

    arr = ft_strjoin(s1, s2);
    free(s1);
	return (arr);
}

void free_2d_array(char **args)
{
    int i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}