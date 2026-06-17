#pragma once

#include "todo/i_storage.h"
#include "model/valueObject/todo.h"

#include <stddef.h>

struct Storage {
    struct IStorage *super;

    const char *path;

    enum Status (*list)(const struct Storage *self, struct Todo todos[], size_t max);
    enum Status (*add)(const struct Storage *self, const char *title);
    enum Status (*edit)(const struct Storage* self, unsigned int id, const char* title, bool completed);
    enum Status (*delete)(const struct Storage *self, unsigned int id);
};

struct Storage *storage_new(const char *path);

void storage_dealloc(struct Storage **storage);
