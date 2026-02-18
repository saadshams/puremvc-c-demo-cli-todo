#pragma once

#include "../valueObject/todo.h"

#include <stdbool.h>

struct IStorage {
    size_t (*list)(const struct IStorage *self, struct Todo todos[], size_t max);
    enum TodoStatus (*add)(const struct IStorage *self, const char *title);
    enum TodoStatus (*edit)(const struct IStorage *self, uint32_t id, const char *title, bool completed);
    enum TodoStatus (*delete)(const struct IStorage *self, uint32_t id);
};
