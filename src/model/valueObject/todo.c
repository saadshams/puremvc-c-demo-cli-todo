#include "todo.h"

#include <stdio.h>

void todo_print(const struct Todo *todo) {
    (void) printf("%u. [%c] %s\n", todo->id, todo->completed ? 'x' : ' ', todo->title);
}
