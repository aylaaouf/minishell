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

char *gc_strjoin_free_a(t_gc *gc, char *s1, char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *joined = gc_malloc(gc, len1 + len2 + 1);
    if (!joined)
        return NULL;
    ft_strcpy(joined, s1);
    ft_strcat(joined, s2);
    return joined;
}

char *ft_strjoin_char_gc(t_gc *gc, char *s, char c)
{
    size_t len = s ? ft_strlen(s) : 0;
    char *new_str = gc_malloc(gc, len + 2);

    if (!new_str)
        return NULL;

    if (s)
        ft_strlcpy(new_str, s, len + 1);
    new_str[len] = c;
    new_str[len + 1] = '\0';

    return new_str;
}

