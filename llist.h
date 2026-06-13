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
 * enum add_node_direction_s - Direction to insert node in list
 * @ADD_NODE_FRONT: Insert at list head
 * @ADD_NODE_REAR: Insert at list tail
 */
typedef enum add_node_direction_s
{
    ADD_NODE_FRONT = 0,
    ADD_NODE_REAR
} add_node_direction_t;

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

typedef int (*node_func_t)(llist_node_t node, unsigned int idx, void *arg);
typedef void (*node_dtor_t)(void *content);
typedef int (*node_cmp_t)(llist_node_t node, void *arg);

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

llist_t *llist_create(llist_mtx_t mt);
size_t llist_size(llist_t const *list);
void *llist_get_head(llist_t const *list);
void *llist_get_tail(llist_t const *list);
void *llist_get_node_at(llist_t const *list, size_t index);
void *llist_find_node(llist_t const *list, node_cmp_t cmp, void *arg);
void *llist_pop(llist_t *list);
llist_node_t *llist_add_node(llist_t *list, void *content,
	add_node_direction_t direction);
void *llist_remove_node(llist_t *list, node_cmp_t cmp, void *arg,
	int free_content, node_dtor_t free_func);
llist_t *llist_destroy(llist_t *list, int free_content, node_dtor_t free_func);
int llist_for_each(llist_t const *list, node_func_t func, void *arg);
void llist_print(llist_t const *list, void (*print_content)(void const *content));
