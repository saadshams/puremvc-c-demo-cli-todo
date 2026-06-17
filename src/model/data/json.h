#pragma once

#include "todo/i_storage.h"

struct JSONStorage {
    struct IStorage super;
    const char *path;
};
