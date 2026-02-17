#pragma once

#include "../../model/valueObject/argument.h"
#include "../../model/valueObject/todo.h"

struct Service {
    struct Argument *argument;
    int argc;
    char **argv;

    const void *context;
    void (*delegate)(const void *context, struct Argument *argument);
    void (*setDelegate)(struct Service *self, const void *context, void (*delegate)(const void *context, struct Argument *argument));

    void (*parse)(const struct Service *self);
    void (*result)(const struct Service *self, const struct Todo *todos, const char *type);
    void (*fault)(const struct Service *self, const char *message);
};

void service_init(struct Service *service, struct Argument *argument, int argc, char **argv);
