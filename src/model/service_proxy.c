#include <stdio.h>

#include "service_proxy.h"

static void onRegister(struct IProxy *self) {
    printf("onRegister\n");
}

void todo_service_proxy_list(struct IProxy *self, struct Command *command) {

}

void todo_service_proxy_add(struct IProxy *self, struct Command *command) {

}

void todo_service_proxy_edit(struct IProxy *self, struct Command *command) {

}

void todo_service_proxy_delete(struct IProxy *self, struct Command *command) {

}

struct IProxy *todo_service_proxy_new(const char **error) {
    struct IProxy *proxy = puremvc_proxy_new(SERVICE_PROXY_NAME, NULL, error);
    proxy->onRegister = onRegister; // override
    return proxy;
}
