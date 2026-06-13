#ifndef LLIST_H
#define LLIST_H

#include <stddef.h>

/**
 * enum llist_mtx_s - Linked list threading mode
 * @MT_SUPPORT_FALSE: No threading support
 * @MT_SUPPORT_TRUE: Threading support enabled
 */
typedef enum llist_mtx_s
{
    MT_SUPPORT_FALSE = 0,
    MT_SUPPORT_TRUE
} llist_mtx_t;

/**
 * enum add_node_direction_s - Node insertion direction
 * @ADD_NODE_FRONT: Insert at the head of the list
 * @ADD_NODE_REAR: Insert at the tail of the list
 */
typedef enum add_node_direction_s
{
    ADD_NODE_FRONT = 0,
    ADD_NODE_REAR
} add_node_direction_t;

/**
 * struct llist_s - Opaque linked list structure
 */
typedef struct llist_s llist_t;

/**
 * typedef llist_node_t - Public node payload pointer
 */
typedef void *llist_node_t;

/**
 * typedef node_func_t - Callback for iterating over list contents
 */
typedef int (*node_func_t)(llist_node_t node, unsigned int idx, void *arg);

/**
 * typedef node_dtor_t - Destructor for node contents
 */
typedef void (*node_dtor_t)(void *content);

/**
 * typedef node_cmp_t - Callback for matching a node during lookup/removal
 */
typedef int (*node_cmp_t)(llist_node_t node, void *arg);

llist_t *llist_create(llist_mtx_t mt);
int llist_size(llist_t const *list);
llist_node_t llist_get_head(llist_t const *list);
llist_node_t llist_get_tail(llist_t const *list);
llist_node_t llist_get_node_at(llist_t const *list, unsigned int index);
llist_node_t llist_find_node(llist_t const *list, node_cmp_t cmp, void *arg);
llist_node_t llist_pop(llist_t *list);
int llist_add_node(llist_t *list, void *content, add_node_direction_t direction);
int llist_remove_node(llist_t *list, node_cmp_t cmp, void *arg,
                      int free_content, node_dtor_t free_func);
llist_t *llist_destroy(llist_t *list, int free_content, node_dtor_t free_func);
int llist_for_each(llist_t const *list, node_func_t func, void *arg);
void llist_print(llist_t const *list, void (*print_content)(void const *content));

#endif /* LLIST_H */
