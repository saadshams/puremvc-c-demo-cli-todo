#pragma once

#include "todo/i_storage.h"

size_t todo_text_storage_size();

struct IStorage *todo_text_storage_init(void *buffer, const char *path);
