/**
 * @file interpreter.c
 * @author James M. Horne (jamesmhorne90@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 */

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "../include/interpreter.h"
#include "../include/stack.h"
#include "../include/memory.h"

#define DEFAULT_MEMORY_SZ 1024 * 1024

/*** private prototypes ***/
memory_t *
load_program (char * p_filename);

void
output (interpreter_t * p_interpreter);

void
input (interpreter_t * p_interpreter);

void
jump_if_zero (interpreter_t * p_interpreter);

void
jump_unconditional (interpreter_t * p_interpreter);

void
comment (interpreter_t * p_interpreter);

void
print_memory (interpreter_t * p_interpreter);

size_t
pull_number (interpreter_t * p_interpreter);

typedef struct interpreter_t
{
    memory_t * p_memory;
    memory_t * p_program;
    stack_t *  p_jump_stack;
} interpreter_t;

/*** public function bodies ***/

interpreter_t *
interpreter_constructor (void)
{
    interpreter_t * p_interpreter = calloc(1, sizeof(interpreter_t));
    stack_t *       p_jump_stack  = stack_constructor();
    memory_t *      p_memory      = memory_constructor(DEFAULT_MEMORY_SZ);

    if ((!p_memory) || (!p_interpreter) || (!p_jump_stack))
    {
        free(p_interpreter);
        p_interpreter = NULL;
        stack_destructor(&p_jump_stack);
        memory_destructor(&p_memory);

        goto EXIT;
    }

    p_interpreter->p_jump_stack = p_jump_stack;
    p_interpreter->p_memory     = p_memory;
    p_interpreter->p_program    = NULL;

    EXIT:

    return p_interpreter;
}

void
interpreter_destructor (interpreter_t ** pp_interpreter)
{

    if ((pp_interpreter) && (*pp_interpreter))
    {
        stack_destructor(&(*pp_interpreter)->p_jump_stack);
        memory_destructor(&(*pp_interpreter)->p_memory);
        memory_destructor(&(*pp_interpreter)->p_program);
        free(*pp_interpreter);
        (*pp_interpreter) = NULL;
    }

}

bool
interpreter_execute (interpreter_t * p_interpreter, char * p_filename)
{
    bool   b_result = false;

    if ((!p_interpreter) || (!p_filename))
    {
        goto EXIT;
    }

    p_interpreter->p_program = load_program(p_filename);
    
    if (!p_interpreter->p_program)
    {
        goto EXIT;
    }

    char command = '\0';
    size_t program_size = memory_get_size(p_interpreter->p_program);

    while (memory_get_pointer(p_interpreter->p_program) < program_size)
    {
        command = memory_get_data(p_interpreter->p_program);

        switch (command)
        {
            case '>':
                memory_increase_pointer(p_interpreter->p_memory);
            break;

            case '<':
                memory_decrease_pointer(p_interpreter->p_memory);
            break;

            case '+':
                memory_increase_data(p_interpreter->p_memory);
            break;

            case '-':
                memory_decrease_data(p_interpreter->p_memory);
            break;

            case '.':
                output(p_interpreter);
            break;

            case ',':
                input(p_interpreter);
            break;

            case '[':
                jump_if_zero(p_interpreter);
            break;

            case ']':
                jump_unconditional(p_interpreter);
            break;

            case '/':
                comment(p_interpreter);
            break;

            case '?':
                print_memory(p_interpreter);
            break;

            case '^':
            break;
        }

        memory_increase_pointer(p_interpreter->p_program);
    }

    b_result = true;

    EXIT:

    return b_result;
}

/*** private function bodies ***/

memory_t *
load_program (char * p_filename)
{
    memory_t * p_program = NULL;
    FILE *     p_file;

    if (!p_filename)
    {
        goto EXIT;
    }

    p_file = fopen(p_filename, "rb");

    if (!p_file)
    {
        goto EXIT;
    }

    fseek(p_file, 0, SEEK_END);
    size_t file_size = ftell(p_file);

    p_program = memory_constructor(file_size);

    if (!p_program)
    {
        goto EXIT;
    }

    rewind(p_file);

    for (size_t idx = 0; idx < file_size; idx++)
    {
        memory_set_data(p_program, (uint8_t) fgetc(p_file));
        memory_increase_pointer(p_program);
    }

    memory_set_pointer(p_program, 0);

    EXIT:

    fclose(p_file);
    return p_program;
}

void
output (interpreter_t * p_interpreter)
{

    if (!p_interpreter)
    {
        return;
    }

    uint8_t data = memory_get_data(p_interpreter->p_memory);

    if ((32 <= data) && (data <= 176))
    {
        printf("%c", data);
    }
    else if ((13 == data) || (10 == data))
    {
        printf("\n");
    }
    else
    {
        printf("\\x%02x", data);
    }

}

void
input (interpreter_t * p_interpreter)
{

    if (!p_interpreter)
    {
        return;
    }

    system("stty raw");

    char input = getchar();

    system("stty cooked");

    memory_set_data(p_interpreter->p_memory, (uint8_t) input);

}

void
jump_if_zero (interpreter_t * p_interpreter)
{
    
    if (!p_interpreter)
    {
        return;
    }

    uint8_t data  = memory_get_data(p_interpreter->p_memory);
    uint8_t start = '[';
    uint8_t end   = ']';

    if (data != 0)
    {
        stack_push(p_interpreter->p_jump_stack,
                   memory_get_pointer(p_interpreter->p_program));

        return;
    }

    int64_t inner_loop_count = -1;

    char command = (char) memory_get_data(p_interpreter->p_program);

    while ((command != end) || (inner_loop_count))
    {
        if (command == start)
        {
            inner_loop_count++;
        }

        if (command == end)
        {
            inner_loop_count--;
        }

        memory_increase_pointer(p_interpreter->p_program);

        command = (char) memory_get_data(p_interpreter->p_program);
    }

}

void
jump_unconditional (interpreter_t * p_interpreter)
{

    if ((!p_interpreter) || (stack_is_empty(p_interpreter->p_jump_stack)))
    {
        return;
    }

    memory_set_pointer(p_interpreter->p_program,
                       stack_pop(p_interpreter->p_jump_stack) - 1);
}

void
comment (interpreter_t * p_interpreter)
{

    if (!p_interpreter)
    {
        return;
    }

    memory_increase_pointer(p_interpreter->p_program);

    uint8_t instruction  = memory_get_data(p_interpreter->p_program);
    size_t  pointer      = memory_get_pointer(p_interpreter->p_program);
    size_t  program_size = memory_get_size(p_interpreter->p_program);

    if ('/' == instruction)
    {

        while (('\n' != instruction) && (pointer < program_size))
        {
            memory_increase_pointer(p_interpreter->p_program);
            instruction = memory_get_data(p_interpreter->p_program);
            pointer++;
        }

    }
    else if ('*' == instruction)
    {

        do
        {
            memory_increase_pointer(p_interpreter->p_program);
            instruction = memory_get_data(p_interpreter->p_program);
            pointer++;
        } while (('*' != instruction) && (pointer < program_size));

        memory_increase_pointer(p_interpreter->p_program);
    }

}

void
print_memory (interpreter_t * p_interpreter)
{

    if (!p_interpreter)
    {
        return;
    }

    memory_increase_pointer(p_interpreter->p_program);
    size_t start = pull_number(p_interpreter);
    memory_increase_pointer(p_interpreter->p_program);
    size_t end = pull_number(p_interpreter);

    printf("\n***** MEMORY *****\n\n");

    for (size_t idx = start; idx < end; idx++)
    {
        printf("%lu\t", idx);
    }

    printf("\n");

    for (size_t idx = start; idx < end; idx++)
    {
        printf("%u\t", memory_get_data_at(p_interpreter->p_memory, idx));
    }
    
    printf("\n");
}

size_t
pull_number (interpreter_t * p_interpreter)
{
    size_t result = 0;

    if (!p_interpreter)
    {
        goto EXIT;
    }

    char    num_str[10] = {0};
    uint8_t ptr         = 0;
    int     character   = memory_get_data(p_interpreter->p_program);
    size_t  program_ptr = memory_get_pointer(p_interpreter->p_program);
    size_t  program_sz  = memory_get_size(p_interpreter->p_program);

    while ((isdigit(character)) && (program_ptr < program_sz) &&
            (ptr < 9))
    {
        num_str[ptr++] = (char) character;
        memory_increase_pointer(p_interpreter->p_program);
        program_ptr++;
        character = memory_get_data(p_interpreter->p_program);
    }

    result = atol(num_str);

    EXIT:

    return result;
}

/*** end of file ***/