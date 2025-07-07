/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:31:55 by ayelasef          #+#    #+#             */
/*   Updated: 2025/05/10 17:34:31 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*gc_malloc(t_gc *gc, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_add(gc, ptr);
	return (ptr);
}

char	*gc_strdup(t_gc *gc, const char *s)
{
	size_t	len;
	char	*copy;

	if (!s)
		return (NULL);
	len = strlen(s);
	copy = (char *)gc_malloc(gc, len + 1);
	if (!copy)
		return (NULL);
	strcpy(copy, s);
	return (copy);
}

void	*gc_realloc(t_gc *gc, void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = gc_malloc(gc, size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
		memcpy(new_ptr, ptr, size);
	return (new_ptr);
}

void	gc_add(t_gc *gc, void *ptr)
{
	t_gc_node	*node;

	if (!ptr || !gc)
		return ;
	node = malloc(sizeof(t_gc_node));
	if (!node)
		return ;
	node->ptr = ptr;
	node->next = gc->list;
	gc->list = node;
}

void	gc_clear(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*tmp;

	if (!gc)
		return ;
	current = gc->list;
	while (current)
	{
		free(current->ptr);
		tmp = current;
		current = current->next;
		free(tmp);
	}
	gc->list = NULL;
}
