#include "service_proxy.h"
#include "storage/i_storage.h"
#include "../application_facade.h"

#include <stdio.h>

static enum Status list(const struct ServiceProxy *self, struct Todo todos[], size_t max) {
    return self->storage->list(self->storage, todos, max);
}

static enum Status add(const struct ServiceProxy *self, const char *title) {
    return self->storage->add(self->storage, title);
}

static enum Status edit(const struct ServiceProxy *self, uint32_t id, const char *title, const bool completed) {
    return self->storage->edit(self->storage, id, title, completed);
}

static enum Status delete(const struct ServiceProxy *self, const uint32_t id) {
    return self->storage->delete(self->storage, id);
}

static const char *help(const struct ServiceProxy *self) {
    (void) self;
    return "Usage:\n"
        "  \x1b[36mtodo list\x1b[0m                                            List all todos\n"
        "  \x1b[36mtodo add \"Buy Milk\"\x1b[0m                                  Add a new todo\n"
        "  \x1b[36mtodo edit 2 -t \"Water the plants\"\x1b[0m                    Edit todo 2 title\n"
        "  \x1b[36mtodo edit 2 -t \"Water the plants\" --completed\x1b[0m        Mark todo 2 completed\n"
        "  \x1b[36mtodo delete 4\x1b[0m                                        Delete todo 4\n"
        "  \x1b[33mtodo --version\x1b[0m                                       Show version\n"
        "  \x1b[33mtodo --help\x1b[0m                                          Show this help\n";
}

static const char *version(const struct ServiceProxy *self) {
    (void) self;
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "\x1b[33m%s (built %s %s)\x1b[0m\n", VERSION, __DATE__, __TIME__);
    return buffer;
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
    serviceProxy->help = help;
    serviceProxy->version = version;

    return serviceProxy;
}
