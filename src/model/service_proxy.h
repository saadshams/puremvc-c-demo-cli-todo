#pragma once

#include "valueObject/todo.h"

#include <puremvc/puremvc.h>

#define ServiceProxy_NAME "ServiceProxy"

struct ServiceProxy {
    struct IProxy *super;

    struct IStorage *storage;
    const char *path;

    enum Status (*list)(const struct ServiceProxy *self, struct Todo todos[], size_t max);
    enum Status (*add)(const struct ServiceProxy *self, const char *title);
    enum Status (*edit)(const struct ServiceProxy* self, uint32_t id, const char* title, bool completed);
    enum Status (*delete)(const struct ServiceProxy *self, uint32_t id);

    const char *(*help)(const struct ServiceProxy *self);
    const char *(*version)(const struct ServiceProxy *self);
};

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data);

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super);
