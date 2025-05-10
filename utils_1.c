/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:34:26 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 17:54:34 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

char *gc_strdup(const char *s)
{
    if (!s)
        return NULL;

    size_t len = strlen(s);
    char *dup = gc_malloc(len + 1);
    if (!dup)
        return (NULL);
    ft_strcpy(dup, s);
    return (dup);
}

char *ft_strndup(char *s, size_t n)
{
    size_t len = strnlen(s, n);
    char *dup = gc_malloc(len + 1);
    if (!dup)
        return NULL;
    
    strncpy(dup, s, len);
    dup[len] = '\0';
    return (dup);
}
