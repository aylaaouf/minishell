/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:20 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 19:07:13 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*dst;
	size_t			i;

	dst = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		dst[i] = (unsigned char)c;
		i++;
	}
	return ((void *)dst);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*b;
	size_t			i;

	b = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		b[i] = 0;
		i++;
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*rsrc;
	unsigned char	*rdst;
	size_t			i;

	if (!src && !dst)
		return (NULL);
	rsrc = (unsigned char *)src;
	rdst = (unsigned char *)dst;
	i = 0;
	if (rsrc != rdst)
	{
		while (i < n)
		{
			rdst[i] = rsrc[i];
			i++;
		}
	}
	return (dst);
}
