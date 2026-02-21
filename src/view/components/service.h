#pragma once

#include "todo/i_service.h"
#include "model/enum/status.h"
#include "model/valueObject/argument.h"

struct Service {
    struct Argument *argument;

    struct IService delegate;

    void (*run)(const struct Service *self, int argc, char **argv);
    void (*result)(const struct Service *self, const void *data, const char *type);
    void (*fault)(const struct Service *self, enum Status status);
    void (*setDelegate)(struct Service *self, struct IService delegate);
};

struct Service *service_init(struct Service *self, struct Argument *argument);
