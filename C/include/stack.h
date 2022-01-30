/**
 * @file stack.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct stack_t stack_t;

stack_t *
stack_constructor (void);

bool
stack_push (stack_t *, size_t);

size_t
stack_pop (stack_t *);

bool
stack_is_empty (stack_t *);

void
stack_destructor (stack_t **);

#endif

/*** end of file ***/
