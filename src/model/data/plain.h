#pragma once

#include "todo/i_storage.h"

struct PlainStorage {
    struct IStorage super;
    const char *path;
};
