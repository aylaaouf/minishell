/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:31:55 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/10 13:17:39 by ayelasef         ###   ########.fr       */
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

void	*gc_realloc(t_gc *gc, void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (gc_malloc(gc, new_size));
	if (new_size == 0)
	{
		gc_free(gc, ptr);
		return (NULL);
	}
	new_ptr = gc_malloc(gc, new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	if (copy_size > 0)
		ft_memcpy(new_ptr, ptr, copy_size);
	gc_free(gc, ptr);
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
