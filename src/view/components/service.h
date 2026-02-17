#pragma once

#include "../../model/valueObject/argument.h"

struct Service {
    struct Argument *command;
    int argc;
    char **argv;

    const void *context;
    void (*delegate)(const void *context, struct Argument *command);
    void (*setDelegate)(struct Service *self, const void *context, void (*delegate)(const void *context, struct Argument *command));

    void (*parse)(const struct Service *self);
};

void service_init(struct Service *service, struct Argument *command, int argc, char **argv);
