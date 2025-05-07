/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:34:26 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/07 16:10:04 by ayelasef         ###   ########.fr       */
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

static char	*ft_strcpy(char *dst, const char *src)
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

char	*ft_strdup(const char *s1)
{
	char	*dst_d;
	size_t	size;

	size = ft_strlen(s1);
	dst_d = malloc(sizeof(char) * size + 1);
	if (!dst_d)
		return (NULL);
	return (ft_strcpy(dst_d, s1));
}

char	*ft_strndup(char *s1, size_t n)
{
	char	*dup;
	size_t	i;

	dup = malloc(sizeof(char) * n + 1);
	if (!dup)
		return (NULL);
	i = 0;
    while (i < n && s1[i])
    {
        dup[i] = s1[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
