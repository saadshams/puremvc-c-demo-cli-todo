#pragma once

#include "../../model/valueObject/Command.h"

struct CLI {
    int argc;
    char **argv;
    void (*delegate)(void *context, struct Command *command);
    void *context;
    void (*parse)(const struct CLI *self, const char **error);
    void (*setDelegate)(struct CLI *self, void *context, void (*delegate)(void *context, struct Command *command));
};

struct CLI *todo_cli_new(int argc, char **argv, const char **error);
void todo_cli_free(struct CLI **cli);
