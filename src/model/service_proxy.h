#pragma once

#include "valueObject/todo.h"

#include "puremvc/puremvc.h"

#define ServiceProxy_NAME "ServiceProxy"

struct ServiceProxy {
    struct IProxy *super;

    struct IStorage *storage;
    const char *path;

    void (*list)(const struct ServiceProxy *self, struct Todo todos[], size_t max);
    void (*add)(const struct ServiceProxy *self, const char *title);
    void (*edit)(const struct ServiceProxy* self, uint32_t id, const char* title, bool completed);
    void (*delete)(const struct ServiceProxy *self, uint32_t id);
    const char *(*help)(const struct ServiceProxy *self);
    const char *(*version)(const struct ServiceProxy *self);
};

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data);

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super);
