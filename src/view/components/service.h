#pragma once

#include "todo/i_delegate.h"
#include "model/enum/status.h"
#include "model/valueObject/argument.h"

struct Service {
    struct Argument *argument;

    struct IDelegate delegate;

    void (*start)(const struct Service *self, int argc, char **argv);
    void (*result)(const struct Service *self, const void *data, const char *type);
    void (*fault)(const struct Service *self, enum Status status);
    void (*setDelegate)(struct Service *self, struct IDelegate delegate);
};

void service_init(struct Service *service, struct Argument *argument);
