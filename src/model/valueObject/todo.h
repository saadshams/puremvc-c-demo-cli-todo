#pragma once

#include <stdbool.h>
#include <stddef.h>

#define TODO_TITLE_MAX   64u
#define TODO_BUFFER_MAX  256  // max length of a single Todo JSON string

struct Todo {
    unsigned int id;
    char title[TODO_TITLE_MAX];
    bool completed;
};

void todo_print(const struct Todo *todo);

bool todo_parseObject(struct Todo *out, const char *object);

size_t todo_parseArray(struct Todo *out, size_t max, const char *json);

size_t todo_parsePointers(struct Todo **out, const char *json);

bool todo_stringifyObject(const struct Todo *todo, char *out, size_t buffer);

size_t todo_stringifyArray(const struct Todo *todos, size_t count, char *buffer, size_t size);

size_t todo_stringifyPointers(struct Todo **todos, char *out, size_t size);
