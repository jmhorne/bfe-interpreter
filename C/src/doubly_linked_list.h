/**
 * @file doubly_linked_list.c
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @version 0.1
 * @date 2021-08-19
 */

#include <stdlib.h>
#include <errno.h>
#include "../include/doubly_linked_list.h"

extern int errno;

/*** private typedef definition ***/
typedef struct node_t
{
    void *          p_data;
    void            (*p_free_func)(void *);
    struct node_t * p_prev;
    struct node_t * p_next;
} node_t;

/*** public typedef definitions ***/
typedef struct doubly_linked_list_t
{
    uint64_t size;
    node_t * p_head;
    node_t * p_tail;
} doubly_linked_list_t;

/*** public function bodies ***/

/**
 * @brief allocates for a new dll
 * 
 * @return doubly_linked_list_t* newly allocated dll. NULL on error
 */
doubly_linked_list_t *
doubly_linked_list_constructor (void)
{
    doubly_linked_list_t * p_new_list = calloc(1, sizeof(doubly_linked_list_t));

    if (NULL != p_new_list)
    {
        p_new_list->p_head = NULL;
        p_new_list->p_tail = NULL;
        p_new_list->size   = 0;
    }

    return p_new_list;
}

/**
 * @brief appends data to a dll.
 * 
 * @param p_list list to append to
 * @param p_data data to append to list
 * @param p_free_func optional function pointer to free data
 * @return true  successfully appended
 * @return false unsuccessfully appended
 */
bool
doubly_linked_list_append (doubly_linked_list_t * p_list, void * p_data,
                           void (*p_free_func)(void *))
{
    node_t * p_new_node = calloc(1, sizeof(node_t));

    if ((NULL == p_list) || (NULL == p_new_node))
    {
        free(p_new_node);
        p_new_node = NULL;
        return false;
    }

    p_new_node->p_data = p_data;
    p_new_node->p_free_func = p_free_func;
    p_new_node->p_next = NULL;

    if (NULL == p_list->p_tail)
    {
        p_new_node->p_prev = NULL;

        p_list->p_head = p_new_node;
        p_list->p_tail = p_new_node;
    }
    else
    {
        p_new_node->p_prev = p_list->p_tail;
        p_list->p_tail->p_next = p_new_node;

        p_list->p_tail = p_new_node;
    }

    p_list->size++;

    return true;
}

/**
 * @brief prepends data to a dll.
 * 
 * @param p_list list to prepend to
 * @param p_data data to prepend to list
 * @param p_free_func optional function pointer to free data
 * @return true  successfully prepended
 * @return false unsuccessfully prepended
 */
bool
doubly_linked_list_prepend (doubly_linked_list_t * p_list, void * p_data,
                            void (*p_free_func)(void *))
{
    node_t * p_new_node = calloc(1, sizeof(node_t));

    if ((NULL == p_list) || (NULL == p_new_node))
    {
        free(p_new_node);
        p_new_node = NULL;
        return false;
    }

    p_new_node->p_data = p_data;
    p_new_node->p_free_func = p_free_func;
    p_new_node->p_prev = NULL;

    if (NULL == p_list->p_head)
    {
        p_new_node->p_next = NULL;

        p_list->p_head = p_new_node;
        p_list->p_tail = p_new_node;
    }
    else
    {
        p_new_node->p_next = p_list->p_head;
        p_list->p_head->p_prev = p_new_node;

        p_list->p_head = p_new_node;
    }

    p_list->size++;

    return true;
}

/**
 * @brief inserts data to a dll.
 * 
 * @param p_list list to insert to
 * @param p_data data to insert to list
 * @param p_free_func optional function pointer to free data
 * @return true  successfully inserted
 * @return false unsuccessfully inserted
 */
bool
doubly_linked_list_insert_at (doubly_linked_list_t * p_list, void * p_data,
                           uint64_t index, void (*p_free_func)(void *))
{

    if (NULL == p_list)
    {
        return false;
    }

    if (0 == index)
    {
        return doubly_linked_list_prepend(p_list, p_data, p_free_func);
    }

    if (p_list->size == index)
    {
        return doubly_linked_list_append(p_list, p_data, p_free_func);
    }

    node_t * p_new_node = calloc(1, sizeof(node_t));
    node_t * p_temp     = NULL;

    if ((NULL == p_new_node) || (index > p_list->size))
    {
        goto EXIT;
    }

    p_new_node->p_data = p_data;
    p_new_node->p_free_func = p_free_func;

    p_temp = p_list->p_head;

    while (index > 0)
    {
        p_temp = p_temp->p_next;
        index--;
    }

    // reset pointers to insert new node between temp->prev and temp
    //  i.e.  previous<->temp becomes previous<->new_node<->temp
    p_new_node->p_next = p_temp;
    p_new_node->p_prev = p_temp->p_prev;
    p_new_node->p_prev->p_next = p_new_node;
    p_temp->p_prev = p_new_node;

    p_list->size++;

    return true;
    
    EXIT:

    free(p_new_node);
    p_new_node = NULL;
    return false;
}

/**
 * @brief Removes the head of a list and returns data at head. Frees underlying
 *          node structure
 * 
 * @param p_list list to remove head from
 * @return void* data at head. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_remove_head (doubly_linked_list_t * p_list)
{
    errno = DLL_ENOERR;

    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    node_t * p_del_node = p_list->p_head;
    void *   p_ret_data = p_del_node->p_data;

    if (1 == p_list->size)
    {
        p_list->p_head = NULL;
        p_list->p_tail = NULL;
    }
    else
    {
        p_list->p_head = p_list->p_head->p_next;
        p_list->p_head->p_prev = NULL;
    }

    free(p_del_node);
    p_del_node = NULL;

    p_list->size--;

    return p_ret_data;
}

/**
 * @brief Removes the tail of a list and returns data at tail. Frees underlying
 *          node structure
 * 
 * @param p_list list to remove tail from
 * @return void* data at tail. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_remove_tail (doubly_linked_list_t * p_list)
{
    errno = DLL_ENOERR;

    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    node_t * p_del_node = p_list->p_tail;
    void *   p_ret_data = p_del_node->p_data;

    if (1 == p_list->size)
    {
        p_list->p_head = NULL;
        p_list->p_tail = NULL;
    }
    else
    {
        p_list->p_tail = p_list->p_tail->p_prev;
        p_list->p_tail->p_next = NULL;
    }

    free(p_del_node);
    p_del_node = NULL;

    p_list->size--;

    return p_ret_data;
}

/**
 * @brief removes data from a list at a given index. Frees underlying node
 * 
 * @param p_list list to remove data from
 * @param index point in list to remove
 * @return void* data at index. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_remove_at (doubly_linked_list_t * p_list, uint64_t index)
{
    errno = DLL_ENOERR;

    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    if (index >= p_list->size)
    {
        errno = DLL_EOUTOFRNGE;
        return NULL;
    }

    if (0 == index)
    {
        return doubly_linked_list_remove_head(p_list);
    }

    if (index == p_list->size - 1)
    {
        return doubly_linked_list_remove_tail(p_list);
    }

    node_t * p_del_node = p_list->p_head;

    while (index > 0)
    {
        p_del_node = p_del_node->p_next;
        index--;
    }

    void * p_ret_data = p_del_node->p_data;

    p_del_node->p_prev->p_next = p_del_node->p_next;
    p_del_node->p_next->p_prev = p_del_node->p_prev;

    free(p_del_node);
    p_del_node = NULL;

    p_list->size--;

    return p_ret_data;
}

/**
 * @brief returns data at head of list without removing it
 * 
 * @param p_list list to get data from
 * @return void* data at head. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_get_head (doubly_linked_list_t * p_list)
{
    errno = DLL_ENOERR;
    
    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    return p_list->p_head->p_data;
}

/**
 * @brief returns data at tail of list without removing it
 * 
 * @param p_list list to get data from
 * @return void* data at tail. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_get_tail (doubly_linked_list_t * p_list)
{
    errno = DLL_ENOERR;

    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    return p_list->p_tail->p_data;
}

/**
 * @brief returns data at a given index without removing it
 * 
 * @param p_list list to get data from
 * @param index point in list to get data from
 * @return void* data at index. If NULL due to empty list, errno set to
 *          DLL_EEMPTYLIST. otherwise errno set to DLL_ENOERR
 */
void *
doubly_linked_list_get_at (doubly_linked_list_t * p_list, uint64_t index)
{
    errno = DLL_ENOERR;
    
    if (doubly_linked_list_is_empty(p_list))
    {
        errno = DLL_EEMPTYLIST;
        return NULL;
    }

    if (0 == index)
    {
        return doubly_linked_list_get_head(p_list);
    }

    if (index == p_list->size - 1)
    {
        return doubly_linked_list_get_tail(p_list);
    }

    if (index >= p_list->size)
    {
        errno = DLL_EOUTOFRNGE;
        return NULL;
    }

    node_t * p_target_node = p_list->p_head;

    while (index > 0)
    {
        p_target_node = p_target_node->p_next;
        index--;
    }

    return p_target_node->p_data;
}

/**
 * @brief returns the amount of elements in the list
 * 
 * @param p_list list to get size of
 * @return uint64_t size of list. 
 */
uint64_t
doubly_linked_list_get_size (doubly_linked_list_t * p_list)
{
    return doubly_linked_list_is_empty(p_list) ? 0 : p_list->size;
}

/**
 * @brief determines if list is empty or not.
 * 
 * @param p_list list to test
 * @return true  is empty
 * @return false is not empty
 */
bool
doubly_linked_list_is_empty (doubly_linked_list_t * p_list)
{
    return ((NULL == p_list) || (0 == p_list->size));
}

/**
 * @brief removes all nodes from a dll
 * 
 * @param p_list dll to clear
 */
void
doubly_linked_list_clear (doubly_linked_list_t * p_list)
{

    if (doubly_linked_list_is_empty(p_list))
    {
        return;
    }

    p_list->p_tail = NULL;

    node_t * p_temp = p_list->p_head;

    while (NULL != p_list->p_head)
    {
        p_list->p_head = p_list->p_head->p_next;

        if (NULL != p_temp->p_free_func)
        {
            p_temp->p_free_func(p_temp->p_data);
        }

        free(p_temp);
        p_temp = p_list->p_head;
    }

    p_list->size = 0;

}

/**
 * @brief clears and deallocates a dll
 * 
 * @param pp_list pointer to dll to destroy
 */
void
doubly_linked_list_destructor (doubly_linked_list_t ** pp_list)
{

    if (NULL != pp_list)
    {
        doubly_linked_list_clear(*pp_list);

        free(*pp_list);
        *pp_list = NULL;
    }

}

/*** end of file ***/
