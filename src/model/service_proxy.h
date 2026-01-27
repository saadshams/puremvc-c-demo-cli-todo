#pragma once

#include "puremvc/puremvc.h"

#define SERVICE_PROXY_NAME "service_proxy"

struct ServiceProxy {
    struct IProxy *proxy;
};

struct IProxy *todo_service_proxy_new(const char **error);
