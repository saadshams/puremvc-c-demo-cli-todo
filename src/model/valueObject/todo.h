#pragma once

#include <stdbool.h>

#define MAX_TODOS        32u
#define TODO_TITLE_MAX   64u

struct Todo {
    unsigned int id;
    char title[TODO_TITLE_MAX];
    bool completed;
};

void todo_print(const struct Todo *todo);
