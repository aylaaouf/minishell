/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 23:04:41 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/17 23:05:13 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return (1);
	return (0);
}

char *gc_substr(t_gc *gc, const char *s, unsigned int start, size_t len)
{
    char    *str;
    size_t  i;
    size_t  s_len;

    if (!s)
        return NULL;

    s_len = ft_strlen(s);
    if (start > s_len)
        return gc_strdup(gc, "");

    if (len > s_len - start)
        len = s_len - start;

    str = gc_malloc(gc, sizeof(char) * (len + 1));
    if (!str)
        return NULL;

    i = 0;
    while (i < len)
    {
        str[i] = s[start + i];
        i++;
    }
    str[i] = '\0';
    return str;
}

static int	ft_count_nb(long n)
{
	int	i;

	i = 0;
	if (n == 0)
		i++;
	else if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n > 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

static char	*ft_rev(char *str)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	j = ft_strlen(str) - 1;
	if (str[0] == '-')
		i = 1;
	while (i < j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	char	*arr;
	int		i;
	long	nn;

	nn = n;
	i = 0;
	arr = malloc(sizeof(char) * ft_count_nb(nn) + 1);
	if (!arr)
		return (NULL);
	if (nn == 0)
		arr[i++] = '0';
	if (n < 0)
	{
		arr[i++] = '-';
		nn *= -1;
	}
	while (nn > 0)
	{
		arr[i] = (nn % 10) + '0';
		nn /= 10;
		i++;
	}
	arr[i] = '\0';
	return (ft_rev(arr));
}
