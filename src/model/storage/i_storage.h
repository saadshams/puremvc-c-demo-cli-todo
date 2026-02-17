#pragma once

#include "../valueObject/todo.h"

#include <stdbool.h>

#define MAX_TODOS 10

struct IStorage {
    bool (*read)(const struct IStorage *self, struct Todo *out, size_t max);
    bool (*write)(struct IStorage *self, const struct Todo *todos, size_t count);

    size_t (*count)(const struct IStorage *self, const struct Todo *todos, size_t max);
    void (*list)(const struct IStorage *self, struct Todo *out);
    bool (*add)(struct IStorage *self, const char *title, struct Todo *out);
    bool (*edit)(struct IStorage *self, unsigned int id, const char *new_title, bool completed, struct Todo *out);
    bool (*delete)(struct IStorage *self, unsigned int id, struct Todo *out);
};
