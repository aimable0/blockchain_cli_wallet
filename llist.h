/*
 * llist.h - Linked list interface
 *
 * This header defines the interface for a generic linked list implementation
 * used throughout the blockchain project.
 */

#ifndef LLIST_H
#define LLIST_H

#include <stddef.h>

/**
 * enum llist_mtx_s - Multi-threading support mode
 * @MT_SUPPORT_FALSE: No multi-threading support
 * @MT_SUPPORT_TRUE: With multi-threading support
 */
typedef enum llist_mtx_s
{
    MT_SUPPORT_FALSE = 0,
    MT_SUPPORT_TRUE
} llist_mtx_t;

/**
 * struct llist_node_s - Node in the linked list
 * @next: Pointer to next node
 * @prev: Pointer to previous node
 * @content: Data pointer stored in the node
 */
typedef struct llist_node_s
{
    struct llist_node_s *next;
    struct llist_node_s *prev;
    void *content;
} llist_node_t;

/**
 * struct llist_s - Linked list structure
 * @head: Pointer to first node (head of list)
 * @tail: Pointer to last node (tail of list)
 * @len: Number of nodes in the list
 */
typedef struct llist_s
{
    llist_node_t *head;
    llist_node_t *tail;
    size_t len;
} llist_t;

/* Function prototypes */

/**
 * llist_create - Create a new linked list
 * @mt: Multi-threading support flag (MT_SUPPORT_FALSE or MT_SUPPORT_TRUE)
 *
 * Return: Pointer to new llist_t or NULL on failure
 */
llist_t *llist_create(llist_mtx_t mt);

/**
 * llist_add_node - Add a new node to the linked list
 * @list: Pointer to the linked list
 * @content: Data pointer to store in the node
 *
 * Return: Pointer to the new node or NULL on failure
 */
llist_node_t *llist_add_node(llist_t *list, void *content);

/**
 * llist_remove_node - Remove a node from the linked list
 * @list: Pointer to the linked list
 * @node: Pointer to the node to remove
 *
 * Return: The data pointer stored in the node or NULL on failure
 */
void *llist_remove_node(llist_t *list, llist_node_t *node);

/**
 * llist_destroy - Destroy the linked list and all its nodes
 * @list: Pointer to the linked list
 * @free_content: If 1, free content data; if 0, don't free content
 * @free_func: Function pointer to free content (can be NULL if free_content is 0)
 *
 * Return: Always NULL
 */
llist_t *llist_destroy(llist_t *list, int free_content,
                       void (*free_func)(void *content));

/**
 * llist_for_each - Iterate over list and apply function to each element
 * @list: Pointer to the linked list
 * @func: Function pointer to apply to each node's content
 *
 * Return: Number of nodes processed or -1 on failure
 */
int llist_for_each(llist_t const *list, int (*func)(void *, unsigned int, void *));

/**
 * llist_print - Print the linked list (for debugging)
 * @list: Pointer to the linked list
 * @print_content: Function pointer to print each node's content
 */
void llist_print(llist_t const *list, void (*print_content)(void const *content));

#endif /* LLIST_H */
