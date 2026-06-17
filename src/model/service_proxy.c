#include "service_proxy.h"

#include "data/storage.h"
#include "application_facade.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void onRegister(struct IProxy *proxy) {}

static void onRemove(struct IProxy *proxy) {}

static enum Status list(const struct ServiceProxy *self, struct Todo todos[], size_t max) {
    return self->storage->list(self->storage, todos, max);
}

static enum Status add(const struct ServiceProxy *self, const struct Argument *argument) {
    const char *title = argument->command.value;
    return self->storage->add(self->storage, title);
}

static enum Status edit(const struct ServiceProxy *self, const struct Argument *argument) {
    const char *id_str = argument->command.value;
    const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;

    const char *title = argument->getFlag(argument, "-t") ? argument->getFlag(argument, "-t") : argument->getFlag(argument, "--title");

    const char *completed_str = argument->getFlag(argument, "-c") ? argument->getFlag(argument, "-c") : argument->getFlag(argument, "--completed");
    const bool completed = completed_str != NULL && strcmp(completed_str, "true") == 0;

    return self->storage->edit(self->storage, id, title, completed);
}

static enum Status delete(const struct ServiceProxy *self, const struct Argument *argument) {
    const char *id_str = argument->command.value;
    const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;

    return id == 0 ? ERR_INVALID_ARGS : self->storage->delete(self->storage, id);
}

static const char *help(const struct ServiceProxy *self) {
    (void) self;
    return "Usage:\n"
        "  \x1b[36mtodo list --file ./todos.json\x1b[0m                                         List all todos\n"
        "  \x1b[36mtodo add \"Buy Milk --file ./todos.json\"\x1b[0m                               Add a new todo\n"
        "  \x1b[36mtodo edit 2 -t \"Water the plants --file ./todos.json\"\x1b[0m                 Edit todo 2 title\n"
        "  \x1b[36mtodo edit 2 -t \"Water the plants\" --completed --file ./todos.json\x1b[0m     Mark todo 2 completed\n"
        "  \x1b[36mtodo delete 4 --file ./todos.json\x1b[0m                                     Delete todo 4\n"
        "  \x1b[33mtodo --version\x1b[0m                                                        Show version\n"
        "  \x1b[33mtodo --help\x1b[0m                                                           Show this help";
}

static const char *version(const struct ServiceProxy *self) {
    (void) self;
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "\x1b[33m%s (built %s %s)\x1b[0m\n", VERSION, __DATE__, __TIME__);
    return buffer;
}

struct IProxy *service_proxy_new() {
    struct IProxy *proxy = puremvc_proxy_new(ServiceProxy_NAME, NULL);
    proxy->onRegister = onRegister;
    proxy->onRemove = onRemove;
    return proxy;
}

struct ServiceProxy *service_proxy_extend(struct ServiceProxy *proxy, struct IProxy *super) {
    proxy->super = super;

    proxy->list = list;
    proxy->add = add;
    proxy->edit = edit;
    proxy->delete = delete;
    proxy->help = help;
    proxy->version = version;

    return proxy;
}
