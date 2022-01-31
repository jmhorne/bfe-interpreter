#include "include/interpreter.h"
#include "include/stack.h"
#include "include/memory.h"

#include <stdio.h>

int
main (void)
{
    interpreter_t * i = interpreter_constructor();

    interpreter_execute(i, "../programs/test");

    interpreter_destructor(&i);

    return 0;
}

/*** end of file ***/
