/**
 * @file memory.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct memory_t memory_t;

memory_t *
memory_constructor (size_t);

bool
memory_increase_data (memory_t *);

bool
memory_decrease_data (memory_t *);

bool
memory_increase_pointer (memory_t *);

bool
memory_decrease_pointer (memory_t *);

size_t
memory_get_pointer (memory_t *);

bool
memory_set_pointer (memory_t *, size_t);

uint8_t
memory_get_data (memory_t *);

bool
memory_set_data (memory_t *, uint8_t);

size_t
memory_get_size (memory_t *);

void
memory_destructor (memory_t **);

#endif

/*** end of file ***/
