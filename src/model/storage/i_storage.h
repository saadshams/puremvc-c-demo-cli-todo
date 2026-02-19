#pragma once

#include "model/enum/status.h"
#include "model/valueObject/todo.h"

#include <stdbool.h>
#include <stddef.h>

struct IStorage {
    enum Status (*list)(const struct IStorage *self, struct Todo todos[], size_t max);
    enum Status (*add)(const struct IStorage *self, const char *title);
    enum Status (*edit)(const struct IStorage *self, uint32_t id, const char *title, bool completed);
    enum Status (*delete)(const struct IStorage *self, uint32_t id);
};
