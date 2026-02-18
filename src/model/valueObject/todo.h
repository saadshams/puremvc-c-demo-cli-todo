#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_TODOS        10
#define TODO_TITLE_MAX   64u

struct Todo {
    uint32_t id;
    char title[TODO_TITLE_MAX];
    bool completed;
};

void todo_print(const struct Todo todos[], const char *type);
