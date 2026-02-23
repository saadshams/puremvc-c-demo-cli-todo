#include "service_proxy.h"
#include "../../include/todo/i_storage.h"
#include "application_facade.h"

#include <stdio.h>
#include <string.h>

static void onRegister(struct IProxy *proxy) {}

static enum Status list(const struct ServiceProxy *self, struct Todo todos[], size_t max) {
    return self->storage->read(self->storage, todos, max);
}

static enum Status add(const struct ServiceProxy *self, const char *title) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->storage->read(self->storage, todos, MAX_TODOS);
    if (status != OK) return status;

    size_t count = 0;
    for (size_t i = 0; i < MAX_TODOS; i++) {
        if (todos[i].id == 0) break;
        count++;
    }
    if (count >= MAX_TODOS) return ERR_FULL;

    struct Todo *todo = &todos[count];
    todo->id = todos[count - 1].id + 1;
    strncpy(todo->title, title, TODO_TITLE_MAX - 1);
    todo->title[TODO_TITLE_MAX - 1] = '\0';
    todo->completed = false;

    return self->storage->write(self->storage, todos);
}

static enum Status edit(const struct ServiceProxy *self, uint32_t id, const char *title, const bool completed) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->storage->read(self->storage, todos, MAX_TODOS);
    if (status != OK) return status;

    bool found = false;
    for (size_t i = 0u; todos[i].id != 0; i++) {
        if (todos[i].id == id) {
            if (title != NULL) {
                strncpy(todos[i].title, title, TODO_TITLE_MAX - 1);
                todos[i].title[TODO_TITLE_MAX - 1] = '\0';
            }
            todos[i].completed = completed;
            found = true;
            break;
        }
    }

    if (found == false) return ERR_NOT_FOUND;

    return self->storage->write(self->storage, todos);
}

static enum Status delete(const struct ServiceProxy *self, const uint32_t id) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->storage->read(self->storage, todos, MAX_TODOS);
    if (status != OK) return status;

    bool found = false;
    size_t index = 0u;

    for (size_t i = 0u; todos[i].id != 0; i++) {
        if (todos[i].id == id) {
            found = true;
        } else {
            if (index != i) {
                todos[index] = todos[i]; /* shift left */
            }
            index++;
        }
    }
    todos[index].id = 0;
    memset(todos[index].title, 0, sizeof(todos[index].title));
    todos[index].completed = false;

    if (found == false) return ERR_NOT_FOUND;

    return self->storage->write(self->storage, todos);
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

struct IProxy *service_proxy_init(void *buffer, const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_init(buffer, name, data);
    proxy->onRegister = onRegister;
    return proxy;
}

struct ServiceProxy *service_proxy_bind(struct ServiceProxy *proxy, struct IProxy *super) {
    proxy->super = super;

    proxy->list = list;
    proxy->add = add;
    proxy->edit = edit;
    proxy->delete = delete;
    proxy->help = help;
    proxy->version = version;

    return proxy;
}
