#pragma once

#include "../../model/valueObject/Command.h"

struct CLI {
    int argc;
    char **argv;

    const void *context;
    void (*delegate)(const void *context, struct Command *command);
    void (*setDelegate)(struct CLI *self, const void *context, void (*delegate)(const void *context, struct Command *command));

    void (*parse)(const struct CLI *self, const char **error);
};

struct CLI *todo_cli_new(int argc, char **argv, const char **error);
void todo_cli_free(struct CLI **cli);
