/**
 * @file interpreter.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "../include/memory.h"

typedef struct interpreter_t interpreter_t;

interpreter_t *
interpreter_constructor (void);

void
interpreter_destructor (interpreter_t **);

bool
interpreter_execute (interpreter_t *, char *);

#endif

/*** end of file ***/
