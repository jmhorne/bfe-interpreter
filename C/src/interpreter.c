/**
 * @file interpreter.c
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#include <stdint.h>

#include "../include/interpreter.h"
#include "../include/stack.h"

typedef struct interpreter_t
{
    uint8_t * p_memory;
    uint8_t * p_program;
    stack_t * p_jump_stack;
} interpreter_t;

/*** end of file ***/