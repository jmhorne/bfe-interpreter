#include "include/interpreter.h"
#include "include/stack.h"

#include <stdio.h>

int
main (void)
{
    stack_t * s = stack_constructor();

    stack_push(s, 500);

    printf("stack is empty %d\n", stack_is_empty(s));
    printf("%lu\n", stack_pop(s));
    printf("stack is empty %d\n", stack_is_empty(s));

    stack_destructor(&s);

    return 0;
}

/*** end of file ***/
