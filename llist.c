#include <stdlib.h>
#include "llist.h"

/**
 * struct llist_node_internal_s - Internal node representation
 * @next: Next node
 * @prev: Previous node
 * @content: Stored content
 */
typedef struct llist_node_internal_s
{
    struct llist_node_internal_s *next;
    struct llist_node_internal_s *prev;
    void *content;
} llist_node_internal_t;

/**
 * struct llist_s - Linked list implementation
 * @head: First node
 * @tail: Last node
 * @size: Number of nodes
 * @mt: Threading mode (unused)
 */
struct llist_s
{
    llist_node_internal_t *head;
    llist_node_internal_t *tail;
    int size;
    llist_mtx_t mt;
};

/**
 * llist_create - Create a new linked list
 * @mt: Threading mode
 *
 * Return: New list or NULL on failure
 */
llist_t *llist_create(llist_mtx_t mt)
{
    llist_t *list;

    list = malloc(sizeof(*list));
    if (list == NULL)
        return (NULL);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->mt = mt;
    return (list);
}

/**
 * llist_size - Get list size
 * @list: List pointer
 *
 * Return: Number of items or 0
 */
int llist_size(llist_t const *list)
{
    if (list == NULL)
        return (0);
    return (list->size);
}

/**
 * llist_get_head - Get head content
 * @list: List pointer
 *
 * Return: Head content or NULL
 */
llist_node_t llist_get_head(llist_t const *list)
{
    if (list == NULL || list->head == NULL)
        return (NULL);
    return (list->head->content);
}

/**
 * llist_get_tail - Get tail content
 * @list: List pointer
 *
 * Return: Tail content or NULL
 */
llist_node_t llist_get_tail(llist_t const *list)
{
    if (list == NULL || list->tail == NULL)
        return (NULL);
    return (list->tail->content);
}

/**
 * llist_get_node_at - Get content at index
 * @list: List pointer
 * @index: Zero-based index
 *
 * Return: Content or NULL
 */
llist_node_t llist_get_node_at(llist_t const *list, unsigned int index)
{
    llist_node_internal_t *node;
    unsigned int i;

    if (list == NULL || index >= (unsigned int)list->size)
        return (NULL);
    node = list->head;
    i = 0;
    while (node != NULL && i < index)
    {
        node = node->next;
        i++;
    }
    if (node == NULL)
        return (NULL);
    return (node->content);
}

/**
 * llist_find_node - Find a matching node content
 * @list: List pointer
 * @cmp: Match callback
 * @arg: Match argument
 *
 * Return: Matching content or NULL
 */
llist_node_t llist_find_node(llist_t const *list, node_cmp_t cmp, void *arg)
{
    llist_node_internal_t *node;

    if (list == NULL || cmp == NULL)
        return (NULL);
    node = list->head;
    while (node != NULL)
    {
        if (cmp(node->content, arg))
            return (node->content);
        node = node->next;
    }
    return (NULL);
}

/**
 * llist_pop - Remove and return head content
 * @list: List pointer
 *
 * Return: Removed content or NULL
 */
llist_node_t llist_pop(llist_t *list)
{
    llist_node_internal_t *head;
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
    list->size--;
    return (content);
}

/**
 * llist_add_node - Add a node to the list
 * @list: List pointer
 * @content: Content to store
 * @direction: Insert direction
 *
 * Return: 0 on success, -1 on failure
 */
int llist_add_node(llist_t *list, void *content, add_node_direction_t direction)
{
    llist_node_internal_t *node;

    if (list == NULL)
        return (-1);

    node = malloc(sizeof(*node));
    if (node == NULL)
        return (-1);
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
    list->size++;
    return (0);
}

/**
 * llist_remove_node - Remove a matching node from the list
 * @list: List pointer
 * @cmp: Match callback
 * @arg: Match argument
 * @free_content: If true, free removed content
 * @free_func: Destructor for content
 *
 * Return: 0 if a node was removed, 1 if no node matched, -1 on error
 */
int llist_remove_node(llist_t *list, node_cmp_t cmp, void *arg,
                      int free_content, node_dtor_t free_func)
{
    llist_node_internal_t *node;
    void *content;

    if (list == NULL || cmp == NULL)
        return (1);

    node = list->head;
    while (node != NULL)
    {
        if (cmp(node->content, arg))
        {
            content = node->content;
            if (node->prev != NULL)
                node->prev->next = node->next;
            else
                list->head = node->next;
            if (node->next != NULL)
                node->next->prev = node->prev;
            else
                list->tail = node->prev;
            if (free_content)
            {
                if (free_func != NULL)
                    free_func(content);
                else
                    free(content);
            }
            free(node);
            list->size--;
            return (0);
        }
        node = node->next;
    }
    return (1);
}

/**
 * llist_destroy - Destroy a list and its nodes
 * @list: List pointer
 * @free_content: If true, free contents
 * @free_func: Content destructor
 *
 * Return: NULL
 */
llist_t *llist_destroy(llist_t *list, int free_content, node_dtor_t free_func)
{
    llist_node_internal_t *node;
    llist_node_internal_t *next;

    if (list == NULL)
        return (NULL);

    node = list->head;
    while (node != NULL)
    {
        next = node->next;
        if (free_content)
        {
            if (free_func != NULL)
                free_func(node->content);
            else
                free(node->content);
        }
        free(node);
        node = next;
    }
    free(list);
    return (NULL);
}

/**
 * llist_for_each - Iterate over the list
 * @list: List pointer
 * @func: Callback
 * @arg: Callback argument
 *
 * Return: 0 on success, 1 if callback stopped iteration, -1 on error
 */
int llist_for_each(llist_t const *list, node_func_t func, void *arg)
{
    llist_node_internal_t *node;
    unsigned int idx;
    int ret;

    if (list == NULL || func == NULL)
        return (-1);

    node = list->head;
    idx = 0;
    while (node != NULL)
    {
        ret = func(node->content, idx, arg);
        if (ret != 0)
            return (ret);
        node = node->next;
        idx++;
    }
    return (0);
}

/**
 * llist_print - Print each node content
 * @list: List pointer
 * @print_content: Print callback
 */
void llist_print(llist_t const *list, void (*print_content)(void const *content))
{
    llist_node_internal_t *node;

    if (list == NULL || print_content == NULL)
        return;

    node = list->head;
    while (node != NULL)
    {
        print_content(node->content);
        node = node->next;
    }
}
