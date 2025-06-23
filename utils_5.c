/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:50:43 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/23 14:00:56 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strjoin_free_a(char *s1, const char *s2)
{
    char *joined;
    size_t len1 = s1 ? ft_strlen(s1) : 0;
    size_t len2 = s2 ? ft_strlen(s2) : 0;

    joined = malloc(len1 + len2 + 1);
    if (!joined)
        return NULL;

    if (s1)
        ft_strlcpy(joined, s1, len1 + 1);
    if (s2)
        ft_strlcpy(joined + len1, s2, len2 + 1);

    free(s1);
    return joined;
}

char *ft_strjoin_char(char *s, char c)
{
    size_t len = s ? ft_strlen(s) : 0;
    char *new_str = malloc(len + 2);

    if (!new_str)
        return NULL;

    if (s)
        ft_strlcpy(new_str, s, len + 1);
    new_str[len] = c;
    new_str[len + 1] = '\0';

    free(s);
    return new_str;
}
