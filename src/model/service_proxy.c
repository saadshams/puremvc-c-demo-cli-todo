#include "service_proxy.h"

#include "storage/i_storage.h"

static size_t count(const struct ServiceProxy *self, const struct Todo *out, size_t max) {
    return self->storage->count(self->storage, out, max);
}

static void list(const struct ServiceProxy *self, struct Todo *out) {
    self->storage->list(self->storage, out);
}

static void add(struct ServiceProxy *self, const char *title, struct Todo *out) {
    self->storage->add(self->storage, title, out);
}

static void edit(struct ServiceProxy *self, unsigned int id, const char *title, bool completed, struct Todo *out) {
    self->storage->edit(self->storage, id, title, completed, out);
}

static void delete(struct ServiceProxy *self, unsigned int id, struct Todo *out) {
    self->storage->delete(self->storage, id, out);
}

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_init(buffer, name, data);
    return proxy;
}

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *serviceProxy, struct IProxy *super) {
    serviceProxy->super = super;

    serviceProxy->count = count;
    serviceProxy->list = list;
    serviceProxy->add = add;
    serviceProxy->edit = edit;
    serviceProxy->delete = delete;

    return serviceProxy;
}
