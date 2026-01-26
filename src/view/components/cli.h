#pragma once

#include "../../model/valueObject/Command.h"

struct CLI {
    struct Command (*parse)(int argc, char **argv, const char **error);
};

struct CLI todo_cli_new();
