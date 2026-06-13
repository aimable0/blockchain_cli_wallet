#include <stdlib.h>
#include "llist.h"

#include <stddef.h>

/**
 * llist_create - Create a new linked list
 * @mt: Multi-threading support flag
 *
 * Return: Pointer to new llist_t or NULL on failure
 */
llist_t *llist_create(llist_mtx_t mt)
{
    llist_t *list;

    (void)mt;
    list = malloc(sizeof(*list));
    if (list == NULL)
        return (NULL);
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
    return (list);
}

/**
 * llist_size - Get list size
 * @list: Pointer to the linked list
 *
 * Return: Number of nodes
 */
size_t llist_size(llist_t const *list)
{
    if (list == NULL)
        return (0);
    return (list->len);
}

/**
 * llist_get_head - Get content at head
 * @list: Pointer to the linked list
 *
 * Return: Content pointer or NULL
 */
void *llist_get_head(llist_t const *list)
{
    if (list == NULL || list->head == NULL)
        return (NULL);
    return (list->head->content);
}

/**
 * llist_get_tail - Get content at tail
 * @list: Pointer to the linked list
 *
 * Return: Content pointer or NULL
 */
void *llist_get_tail(llist_t const *list)
{
    if (list == NULL || list->tail == NULL)
        return (NULL);
    return (list->tail->content);
}

/**
 * llist_get_node_at - Get content at index
 * @list: Pointer to the linked list
 * @index: Index in list
 *
 * Return: Content pointer or NULL
 */
void *llist_get_node_at(llist_t const *list, size_t index)
{
    llist_node_t *current;
    size_t i;

    if (list == NULL || index >= list->len)
        return (NULL);
    current = list->head;
    i = 0;
    while (current != NULL && i < index)
    {
        current = current->next;
        i++;
    }
    return (current != NULL ? current->content : NULL);
}

/**
 * llist_find_node - Find a node by predicate
 * @list: Pointer to linked list
 * @cmp: Predicate function
 * @arg: User argument
 *
 * Return: Content pointer of matching node or NULL
 */
void *llist_find_node(llist_t const *list, node_cmp_t cmp, void *arg)
{
    llist_node_t *current;

    if (list == NULL || cmp == NULL)
        return (NULL);
    current = list->head;
    while (current != NULL)
    {
        if (cmp(current, arg))
            return (current->content);
        current = current->next;
    }
    return (NULL);
}

/**
 * llist_pop - Remove and return content at head
 * @list: Pointer to the linked list
 *
 * Return: Content pointer or NULL
 */
void *llist_pop(llist_t *list)
{
    llist_node_t *head;
    void *content;

    if (list == NULL || list->head == NULL)
        return (NULL);
    head = list->head;
    content = head->content;
    list->head = head->next;
    if (list->head != NULL)
        list->head->prev = NULL;
    else
        list->tail = NULL;
    free(head);
    list->len--;
    return (content);
}

/**
 * llist_add_node - Add a new node to the linked list
 * @list: Pointer to the linked list
 * @content: Data pointer to store in the node
 * @direction: Where to insert the node
 *
 * Return: Pointer to the new node or NULL on failure
 */
llist_node_t *llist_add_node(llist_t *list, void *content,
                             add_node_direction_t direction)
{
    llist_node_t *node;

    if (!list)
        return (NULL);

    node = malloc(sizeof(*node));
    if (!node)
        return (NULL);

    node->content = content;
    if (direction == ADD_NODE_FRONT)
    {
        node->prev = NULL;
        node->next = list->head;
        if (list->head != NULL)
            list->head->prev = node;
        else
            list->tail = node;
        list->head = node;
    }
    else
    {
        node->next = NULL;
        node->prev = list->tail;
        if (list->tail != NULL)
            list->tail->next = node;
        else
            list->head = node;
        list->tail = node;
    }
    list->len++;

    return (node);
}

/**
 * llist_remove_node - Remove a node from the linked list
 * @list: Pointer to the linked list
 * @cmp: Predicate to match the node
 * @arg: Argument for predicate
 * @free_content: If nonzero, free content
 * @free_func: Destructor for content
 *
 * Return: The data pointer stored in the node or NULL on failure
 */
void *llist_remove_node(llist_t *list, node_cmp_t cmp, void *arg,
                        int free_content, node_dtor_t free_func)
{
    llist_node_t *current;
    void *content;

    if (list == NULL || cmp == NULL)
        return (NULL);

    current = list->head;
    while (current != NULL)
    {
        if (cmp(current, arg))
            break;
        current = current->next;
    }
    if (current == NULL)
        return (NULL);
    content = current->content;
    if (current->prev != NULL)
        current->prev->next = current->next;
    else
        list->head = current->next;
    if (current->next != NULL)
        current->next->prev = current->prev;
    else
        list->tail = current->prev;
    if (free_content && free_func != NULL)
        free_func(content);
    free(current);
    list->len--;
    return (content);
}

*llist_destroy - Destroy the linked list and all its nodes
                     *@list : Pointer to the linked list
                                  *@free_content : If 1,
    free content data;
if
    0, don't free content
               *@free_func : Function pointer to free
                             content(can be NULL if free_content is 0) *
                             *Return : Always NULL
                                           * /
                             llist_t *llist_destroy(llist_t *list, int free_content, node_dtor_t free_func)
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
 * @arg: User argument
 *
 * Return: Number of nodes processed or -1 on failure
 */
int llist_for_each(llist_t const *list, node_func_t func, void *arg)
{
    llist_node_t *current;
    unsigned int idx;

    if (!list || !func)
        return (-1);

    current = list->head;
    idx = 0;
    while (current)
    {
        if (func(current->content, idx, arg) != 0)
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
