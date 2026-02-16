#include "service_proxy.h"

#include <stdio.h>

static bool read(struct IProxy *self, struct Command *command) {
    return false;
}

static bool write(struct IProxy *self, struct Command *command) {
    return false;
}

static void list(struct IProxy *self, struct Command *command) {
    // struct ServiceProxy *serviceProxy = (struct ServiceProxy *) self;
    printf("list\n");
}

static void add(struct IProxy *self, struct Command *command) {

}

static void edit(struct IProxy *self, struct Command *command) {

}

static void delete(struct IProxy *self, struct Command *command) {

}

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_init(buffer, name, data);
    return proxy;
}

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super) {
    serviceProxy->super = super;

    serviceProxy->read = read;
    serviceProxy->write = write;
    serviceProxy->list = list;
    serviceProxy->add = add;
    serviceProxy->edit = edit;
    serviceProxy->delete = delete;

    return serviceProxy;
}
