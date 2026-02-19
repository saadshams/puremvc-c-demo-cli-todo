#pragma once

#include "todo/i_storage.h"

size_t todo_json_storage_size();

struct IStorage *todo_json_storage_init(void *buffer, const char *path);
