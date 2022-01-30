/**
 * @file stack.c
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#include <stdlib.h>

#include "../include/stack.h"
#include "../include/doubly_linked_list.h"

typedef struct stack_t
{
    doubly_linked_list_t * p_head;
} stack_t;

stack_t *
stack_constructor (void)
{
    stack_t * p_stack = calloc(1, sizeof(stack_t));

    if (p_stack)
    {
        p_stack->p_head = doubly_linked_list_constructor();

        if (!p_stack->p_head)
        {
            free(p_stack);
        }

    }

    return p_stack;
}

bool
stack_push (stack_t * p_stack, size_t data)
{
    bool b_result = false;
    size_t *        p_data_holder = calloc(1, sizeof(size_t));

    if ((!p_stack) || (!p_data_holder) || (0 == data))
    {
        goto EXIT;
    }

    *p_data_holder = data;

    b_result = doubly_linked_list_prepend(p_stack->p_head,
                    (void *) p_data_holder, free);

    EXIT:

    return b_result;
}

size_t
stack_pop (stack_t * p_stack)
{
    size_t return_data = 0;

    if ((!p_stack) || (!p_stack->p_head))
    {
        goto EXIT;
    }

    void * p_return = doubly_linked_list_remove_head(p_stack->p_head);

    if (p_return)
    {
        return_data = *(size_t *) p_return;

        free(p_return);
        p_return = NULL;
    }

    EXIT:

    return return_data;
}

bool
stack_is_empty (stack_t * p_stack)
{
    bool b_result = false;

    if ((!p_stack) || (!p_stack->p_head))
    {
        goto EXIT;
    }

    b_result = doubly_linked_list_is_empty(p_stack->p_head);

    EXIT:

    return b_result;
}

void
stack_destructor (stack_t ** pp_stack)
{

    if ((pp_stack) && (*pp_stack))
    {
        doubly_linked_list_destructor(&(*pp_stack)->p_head);
        free(*pp_stack);
        *pp_stack = NULL;
    }

}

/*** end of file ***/
