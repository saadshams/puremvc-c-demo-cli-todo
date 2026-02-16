#pragma once

#include "valueObject/command.h"

#include "puremvc/puremvc.h"

#define ServiceProxy_NAME "ServiceProxy"

struct ServiceProxy {
    struct IProxy *super;

    struct IStorage *storage; // Strategy Pattern, Dependency Injection
    const char *path;

    bool (*read)(struct IProxy *self, struct Command *command);
    bool (*write)(struct IProxy *self, struct Command *command);
    void (*list)(struct IProxy *self, struct Command *command);
    void (*add)(struct IProxy *self, struct Command *command);
    void (*edit)(struct IProxy *self, struct Command *command);
    void (*delete)(struct IProxy *self, struct Command *command);
};

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data);

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super);
