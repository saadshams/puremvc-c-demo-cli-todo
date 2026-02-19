#pragma once

#include "../../model/enum/status.h"
#include "../../model/valueObject/argument.h"

struct Service {
    struct Argument *argument;

    const void *context;
    void (*delegate)(const void *context, struct Argument *argument);
    void (*setDelegate)(struct Service *self, const void *context, void (*delegate)(const void *context, struct Argument *argument));

    void (*start)(const struct Service *self, int argc, char **argv);
    void (*result)(const struct Service *self, const void *data, const char *type);
    void (*fault)(const struct Service *self, enum Status status);
};

void service_init(struct Service *service, struct Argument *argument);
