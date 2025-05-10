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

t_gc *g_gc = NULL;

void *gc_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        return NULL;
    gc_add(ptr);
    return ptr;
}

void gc_add(void *ptr)
{
    if (!ptr)
        return;

    t_gc *new_node = malloc(sizeof(t_gc));
    if (!new_node)
        return;

    new_node->ptr = ptr;
    new_node->next = g_gc;
    g_gc = new_node;
}

void gc_free(void *ptr)
{
    t_gc *prev = NULL;
    t_gc *current = g_gc;

    while (current)
    {
        if (current->ptr == ptr)
        {
            free(ptr);
            if (prev)
                prev->next = current->next;
            else
                g_gc = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void gc_clear(void)
{
    t_gc *current = g_gc;
    while (current)
    {
        free(current->ptr);
        t_gc *temp = current;
        current = current->next;
        free(temp);
    }
    g_gc = NULL;
}
