/**
 * @file memory.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../include/memory.h"

typedef struct memory_t
{
    bool      b_ptr_rollover;
    bool      b_data_rollover;
    uint8_t * p_blocks;
    size_t    size;
    size_t    pointer;
} memory_t;

memory_t *
memory_constructor (bool b_ptr_rollover, bool b_data_rollover, size_t size)
{
    memory_t * p_memory = calloc(1, sizeof(memory_t));
    uint8_t *  p_blocks = calloc(size, sizeof(uint8_t));

    if ((0 == size) || (!p_memory) || (!p_blocks))
    {
        free(p_memory);
        p_memory = NULL;
        free(p_blocks);
        p_blocks = NULL;
        goto EXIT;
    }

    p_memory->b_ptr_rollover  = b_ptr_rollover;
    p_memory->b_data_rollover = b_data_rollover;
    p_memory->p_blocks        = p_blocks;
    p_memory->size            = size;
    p_memory->pointer         = 0;

    EXIT:

    return p_memory;
}

/*** end of file ***/
