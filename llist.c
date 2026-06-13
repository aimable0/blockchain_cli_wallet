/*
 * llist.c - Linked list implementation
 *
 * This file provides a generic linked list implementation for the blockchain project.
 */

#include <stdlib.h>
#include <string.h>
#include "llist.h"

/**
 * llist_create - Create a new linked list
 * @mt: Multi-threading support flag
 *
 * Return: Pointer to new llist_t or NULL on failure
 */
llist_t *llist_create(llist_mtx_t mt)
{
    llist_t *list;

    (void)mt; /* unused for now */

    list = malloc(sizeof(llist_t));
    if (!list)
        return (NULL);

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;

    return (list);
}

/**
 * llist_add_node - Add a new node to the linked list (at tail)
 * @list: Pointer to the linked list
 * @content: Data pointer to store in the node
 *
 * Return: Pointer to the new node or NULL on failure
 */
llist_node_t *llist_add_node(llist_t *list, void *content)
{
    llist_node_t *node;

    if (!list)
        return (NULL);

    node = malloc(sizeof(llist_node_t));
    if (!node)
        return (NULL);

    node->content = content;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->len++;

    return (node);
}

/**
 * llist_remove_node - Remove a node from the linked list
 * @list: Pointer to the linked list
 * @node: Pointer to the node to remove
 *
 * Return: The data pointer stored in the node or NULL on failure
 */
void *llist_remove_node(llist_t *list, llist_node_t *node)
{
    void *content;

    if (!list || !node)
        return (NULL);

    content = node->content;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    list->len--;
    free(node);

    return (content);
}

/**
 * llist_destroy - Destroy the linked list and all its nodes
 * @list: Pointer to the linked list
 * @free_content: If 1, free content data; if 0, don't free content
 * @free_func: Function pointer to free content (can be NULL if free_content is 0)
 *
 * Return: Always NULL
 */
llist_t *llist_destroy(llist_t *list, int free_content,
                       void (*free_func)(void *content))
{
    llist_node_t *current, *next;

    if (!list)
        return (NULL);

    current = list->head;
    while (current)
    {
        next = current->next;
        if (free_content && free_func)
            free_func(current->content);
        free(current);
        current = next;
    }

    free(list);
    return (NULL);
}

/**
 * llist_for_each - Iterate over list and apply function to each element
 * @list: Pointer to the linked list
 * @func: Function pointer to apply to each node's content
 *
 * Return: Number of nodes processed or -1 on failure
 */
int llist_for_each(llist_t const *list, int (*func)(void *, unsigned int, void *))
{
    llist_node_t *current;
    unsigned int idx;

    if (!list || !func)
        return (-1);

    current = list->head;
    idx = 0;
    while (current)
    {
        if (func(current->content, idx, NULL) != 0)
            return (-1);
        current = current->next;
        idx++;
    }

    return ((int)list->len);
}

/**
 * llist_print - Print the linked list (for debugging)
 * @list: Pointer to the linked list
 * @print_content: Function pointer to print each node's content
 */
void llist_print(llist_t const *list, void (*print_content)(void const *content))
{
    llist_node_t *current;

    if (!list || !print_content)
        return;

    current = list->head;
    while (current)
    {
        print_content(current->content);
        current = current->next;
    }
}
