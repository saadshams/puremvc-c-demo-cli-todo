#pragma once

#include "../../model/valueObject/command.h"

struct Service {
    struct Command *command;
    int argc;
    char **argv;

    const void *context;
    void (*delegate)(const void *context, struct Command *command);
    void (*setDelegate)(struct Service *self, const void *context, void (*delegate)(const void *context, struct Command *command));

    void (*parse)(const struct Service *self);
};

void service_init(struct Service *service, struct Command *command, int argc, char **argv);
