#pragma once

#include "../valueObject/todo.h"

#include <stdbool.h>

struct IStorage {
    size_t (*read)(const struct IStorage *self, struct Todo todos[]);
    bool (*write)(const struct IStorage *self, const struct Todo todos[], size_t count);

    size_t (*count)(const struct IStorage *self, const struct Todo *todos);

    size_t (*list)(const struct IStorage *self, struct Todo todos[]);

    enum TodoStatus (*add)(const struct IStorage *self, struct Todo todos[], const char *title);
    enum TodoStatus (*edit)(const struct IStorage *self, struct Todo todos[], uint32_t id, const char *title, bool completed);
    enum TodoStatus (*delete)(const struct IStorage *self, struct Todo todos[], uint32_t id);
};
