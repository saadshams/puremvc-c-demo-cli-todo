#include <stdio.h>

#include "service_proxy.h"

static void onRegister(struct IProxy *self) {

}

struct IProxy *todo_service_proxy_new(const char **error) {
    struct IProxy *proxy = puremvc_proxy_new(SERVICE_PROXY_NAME, NULL, error);
    proxy->onRegister = onRegister;
    return proxy;
}
