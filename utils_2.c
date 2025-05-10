/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:20 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 17:58:48 by ayelasef         ###   ########.fr       */
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

void *ft_calloc(size_t count, size_t size)
{
    void *arr;
    size_t max;
	
	max= -1;
    if (size != 0 && (count > max / size))
        return (NULL);

    arr = gc_malloc(count * size);
    if (!arr)
        return (NULL);
    ft_bzero(arr, count * size);
    return (arr);
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

void *ft_realloc(void *ptr, size_t new_size)
{
	void *new_ptr;

    if (new_size == 0)
    {
        gc_free(ptr);
        return NULL;
    }
    if (!ptr)
        return (gc_malloc(new_size));
    new_ptr = gc_malloc(new_size);
    if (!new_ptr)
        return (NULL);
    ft_memcpy(new_ptr, ptr, new_size);
    gc_free(ptr);
    return (new_ptr);
}
