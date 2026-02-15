#include <stdio.h>

#include "service_proxy.h"

static void onRegister(struct IProxy *self) {
    printf("onRegister: Service Proxy\n");
}

static void list(struct IProxy *self, struct Command *command) {
    // struct ServiceProxy *serviceProxy = (struct ServiceProxy *) self;
}

static void add(struct IProxy *self, struct Command *command) {

}

static void edit(struct IProxy *self, struct Command *command) {

}

static void delete(struct IProxy *self, struct Command *command) {

}

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_init(buffer, name, data);
    proxy->onRegister = onRegister; // override
    return proxy;
}

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *proxy) {
    serviceProxy->super = proxy;

    serviceProxy->list = list;
    serviceProxy->add = add;
    serviceProxy->edit = edit;
    serviceProxy->delete = delete;

    return serviceProxy;
}
