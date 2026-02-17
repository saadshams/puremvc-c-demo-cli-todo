#pragma once

#include "valueObject/todo.h"

#include "puremvc/puremvc.h"

#define ServiceProxy_NAME "ServiceProxy"

struct ServiceProxy {
    struct IProxy *super;

    struct IStorage *storage;
    const char *path;

    void (*list)(const struct ServiceProxy *self, struct Todo *out);
    void (*add)(struct ServiceProxy *self, const char *title, struct Todo *out);
    void (*edit)(struct ServiceProxy *self, unsigned int id, const char *title, bool completed, struct Todo *out);
    void (*delete)(struct ServiceProxy *self, unsigned int id, struct Todo *out);
};

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data);

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super);
