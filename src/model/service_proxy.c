#include "service_proxy.h"

#include <stdio.h>

static bool load(struct ServiceProxy *self, const char *path, struct Todo *todos) {
    char line[128];
    size_t index = 0;
    unsigned int completed;

    FILE* file = fopen(path, "r");
    if (file == NULL) return false;

    while (fgets(line, sizeof(line), file) != NULL && index < MAX_TODOS) {
        struct Todo *todo = &todos[index];
        if (sscanf(line, "%u|%u|%63[^\n]", &todo->id, &completed, todo->title) == 3) {
            todo->completed = completed != 0u;
            index++;
        }
    }
    todos[index].id = 0u; // Sentinel

    (void) fclose(file);
    return true;
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

    serviceProxy->load = load;
    serviceProxy->list = list;
    serviceProxy->add = add;
    serviceProxy->edit = edit;
    serviceProxy->delete = delete;

    return serviceProxy;
}
