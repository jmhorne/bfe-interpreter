/**
 * @file doubly_linked_list.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @version 0.1
 * @date 2021-08-19
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdbool.h>
#include <stdint.h>

#define DLL_ENOERR 0
#define DLL_ELISTERR 1
#define DLL_EEMPTYLIST 2
#define DLL_EOUTOFRNGE 3

typedef struct doubly_linked_list_t doubly_linked_list_t;

doubly_linked_list_t *
doubly_linked_list_constructor (void);

bool
doubly_linked_list_append (doubly_linked_list_t * p_list, void * p_data,
                           void (*p_free_func)(void *));

bool
doubly_linked_list_prepend (doubly_linked_list_t * p_list, void * p_data,
                            void (*p_free_func)(void *));

bool
doubly_linked_list_insert_at (doubly_linked_list_t * p_list, void * p_data,
                           uint64_t index, void (*p_free_func)(void *));

void *
doubly_linked_list_remove_head (doubly_linked_list_t * p_list);

void *
doubly_linked_list_remove_tail (doubly_linked_list_t * p_list);

void *
doubly_linked_list_remove_at (doubly_linked_list_t * p_list, uint64_t index);

void *
doubly_linked_list_get_head (doubly_linked_list_t * p_list);

void *
doubly_linked_list_get_tail (doubly_linked_list_t * p_list);

void *
doubly_linked_list_get_at (doubly_linked_list_t * p_list, uint64_t index);

uint64_t
doubly_linked_list_get_size (doubly_linked_list_t * p_list);

bool
doubly_linked_list_is_empty (doubly_linked_list_t * p_list);

void
doubly_linked_list_clear (doubly_linked_list_t * p_list);

void
doubly_linked_list_destructor (doubly_linked_list_t ** pp_list);

#endif

/*** end of file ***/
