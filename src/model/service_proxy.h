#pragma once

#include "puremvc/puremvc.h"
#include "valueObject/Command.h"

#define SERVICE_PROXY_NAME "service_proxy"

struct ServiceProxy {
    struct IProxy base;

    const char* name;
    void* data;

    void (*list)(struct IProxy *self, struct Command *command);
    void (*add)(struct IProxy *self, struct Command *command);
    void (*edit)(struct IProxy *self, struct Command *command);
};

static void todo_service_proxy_delete(struct IProxy *self, struct Command *command);

struct IProxy *todo_service_proxy_new(const char **error);
