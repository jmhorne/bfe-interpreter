/**
 * @file memory.h
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#include <stdlib.h>

#include "../include/memory.h"

typedef struct memory_t
{
    uint8_t * p_blocks;
    size_t    size;
    size_t    pointer;
} memory_t;

/*** public function bodies ***/

memory_t *
memory_constructor (size_t size)
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

    p_memory->p_blocks        = p_blocks;
    p_memory->size            = size;
    p_memory->pointer         = 0;

    EXIT:

    return p_memory;
}

bool
memory_increase_data (memory_t * p_memory)
{
    bool b_result = false;

    if (!p_memory)
    {
        goto EXIT;
    }

    p_memory->p_blocks[p_memory->pointer]++;
    b_result = true;

    EXIT:

    return b_result;
}

bool
memory_decrease_data (memory_t * p_memory)
{
    bool b_result = false;

    if (!p_memory)
    {
        goto EXIT;
    }

    p_memory->p_blocks[p_memory->pointer]--;
    b_result = true;

    EXIT:

    return b_result;
}

bool
memory_increase_pointer (memory_t * p_memory)
{
    bool b_result = false;

    if (!p_memory)
    {
        goto EXIT;
    }

    p_memory->pointer++;

    if (p_memory->pointer >= p_memory->size)
    {
        p_memory->pointer = 0;
    }

    b_result = true;

    EXIT:

    return b_result;
}

bool
memory_decrease_pointer (memory_t * p_memory)
{
    bool b_result = false;

    if (!p_memory)
    {
        goto EXIT;
    }

    if (0 == p_memory->pointer)
    {
        p_memory->pointer = p_memory->size - 1;
    }
    else
    {
        p_memory->pointer--;
    }

    b_result = true;

    EXIT:

    return b_result;
}

size_t
memory_get_pointer (memory_t * p_memory)
{
    size_t pointer = 0;

    if (!p_memory)
    {
        goto EXIT;
    }

    pointer = p_memory->pointer;

    EXIT:

    return pointer;
}

bool
memory_set_pointer (memory_t * p_memory, size_t position)
{
    bool b_result = false;

    if ((!p_memory) || (position >= p_memory->size))
    {
        goto EXIT;
    }

    p_memory->pointer = position;
    b_result = true;

    EXIT:

    return b_result;
}

uint8_t
memory_get_data (memory_t * p_memory)
{
    uint8_t result = 0;

    if (!p_memory)
    {
        goto EXIT;
    }

    result = p_memory->p_blocks[p_memory->pointer];

    EXIT:

    return result;
}

bool
memory_set_data (memory_t * p_memory, uint8_t data)
{
    bool b_result = false;

    if ((!p_memory) || (!p_memory->p_blocks))
    {
        goto EXIT;
    }

    p_memory->p_blocks[p_memory->pointer] = data;

    EXIT:

    return b_result;
}

size_t
memory_get_size (memory_t * p_memory)
{
    size_t size = 0;

    if (!p_memory)
    {
        goto EXIT;
    }

    size = p_memory->size;

    EXIT:

    return size;
}

void
memory_destructor (memory_t ** pp_memory)
{

    if ((pp_memory) && (*pp_memory))
    {
        free((*pp_memory)->p_blocks);
        (*pp_memory)->p_blocks = NULL;
        free(*pp_memory);
        *pp_memory = NULL;
    }

}

/*** end of file ***/
