#include "service_proxy.h"

#include "storage/i_storage.h"

static void list(const struct ServiceProxy *self, struct Todo todos[], size_t max) {
    self->storage->list(self->storage, todos, max);
}

static void add(const struct ServiceProxy *self, const char *title) {
    self->storage->add(self->storage, title);
}

static void edit(const struct ServiceProxy *self, uint32_t id, const char *title, const bool completed) {
    self->storage->edit(self->storage, id, title, completed);
}

static void delete(const struct ServiceProxy *self, uint32_t id) {
    self->storage->delete(self->storage, id);
}

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_init(buffer, name, data);
    return proxy;
}

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super) {
    serviceProxy->super = super;

    serviceProxy->list = list;
    serviceProxy->add = add;
    serviceProxy->edit = edit;
    serviceProxy->delete = delete;

    return serviceProxy;
}
