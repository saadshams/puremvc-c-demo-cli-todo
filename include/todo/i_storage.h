#pragma once

#include "model/enum/status.h"
#include "model/valueObject/todo.h"

#include <stddef.h>

struct IStorage {
    enum Status (*read)(const struct IStorage *self, struct Todo todos[], size_t max);
    enum Status (*write)(const struct IStorage *self, const struct Todo todos[]);
};
