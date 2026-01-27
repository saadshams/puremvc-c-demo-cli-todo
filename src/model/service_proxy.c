#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "service_proxy.h"

static void onRegister(struct IProxy *self) {
    printf("onRegister\n");
}

static void list(struct IProxy *self, struct Command *command) {

}

static void add(struct IProxy *self, struct Command *command) {

}

// Function pointer implementation
static void edit(struct IProxy *self, struct Command *command) {

}

// Classic implementation for demonstration
static void todo_service_proxy_delete(struct IProxy *self, struct Command *command) {

}

static struct ServiceProxy *init(struct ServiceProxy *proxy) {
    if (proxy == NULL) return NULL;
    puremvc_proxy_init(&proxy->base);
    proxy->base.onRegister = onRegister; // override
    proxy->list = list;
    proxy->add = add;
    proxy->edit = edit;
    return proxy;
}

static struct ServiceProxy *alloc(const char **error) {
    struct ServiceProxy *proxy = malloc(sizeof(struct ServiceProxy));
    if (proxy == NULL) return *error = "[Todo::ServiceProxy::alloc] Error: Failed to allocate Proxy", NULL;

    memset(proxy, 0, sizeof(struct ServiceProxy));

    proxy->base.notifier = puremvc_notifier_new(error);

    proxy->name = strdup(SERVICE_PROXY_NAME);
    proxy->data = NULL;

    return proxy;
}

struct IProxy *todo_service_proxy_new(const char **error) {
    return (struct IProxy *) init(alloc(error));
}
