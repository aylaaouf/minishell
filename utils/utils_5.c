/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:50:43 by ayelasef          #+#    #+#             */
/*   Updated: 2025/06/29 23:22:44 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	ft_count_digits(long n)
{
	int	count = 0;

	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++; // for '-'
		n = -n;
	}
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	ft_reverse_str(char *str, int len)
{
	int		start = 0;
	int		end = len - 1;
	char	tmp;

	if (str[0] == '-')
		start = 1;
	while (start < end)
	{
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
		start++;
		end--;
	}
}

char	*ft_itoa_gc(t_gc *gc, int n)
{
	char	*str;
	int		i = 0;
	long	nb = n;

	str = gc_malloc(gc, sizeof(char) * (ft_count_digits(nb) + 1));
	if (!str)
		return (NULL);

	if (nb == 0)
		str[i++] = '0';
	if (nb < 0)
	{
		str[i++] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	str[i] = '\0';
	ft_reverse_str(str, i);
	return (str);
}

