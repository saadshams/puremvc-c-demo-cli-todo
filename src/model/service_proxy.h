#pragma once

#include "valueObject/todo.h"
#include "valueObject/argument.h"

#include <puremvc/puremvc.h>

#define ServiceProxy_NAME "ServiceProxy"

struct ServiceProxy {
    struct IProxy *super;

    struct Storage *storage;

    enum Status (*list)(const struct ServiceProxy *self, struct Todo todos[], size_t max);
    enum Status (*add)(const struct ServiceProxy *self, const struct Argument *argument);
    enum Status (*edit)(const struct ServiceProxy* self, const struct Argument *argument);
    enum Status (*delete)(const struct ServiceProxy *self, const struct Argument *argument);

    const char *(*help)(const struct ServiceProxy *self);
    const char *(*version)(const struct ServiceProxy *self);
};

struct IProxy *service_proxy_new();

struct ServiceProxy *service_proxy_extend(struct ServiceProxy *proxy, struct IProxy *super);
