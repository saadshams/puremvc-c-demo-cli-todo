#include "service_command.h"

#include "../model/service_proxy.h"
#include "../model/valueObject/argument.h"
#include "../model/storage/todo_text.h"
#include "../model/storage/todo_json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void execute(const struct ICommand *self, struct INotification *notification) {
    printf("service command\n");

    const struct IFacade *facade = self->getNotifier(self)->getFacade(self->getNotifier(self));
    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);

    // Strategy Pattern, Dependency Injection
    proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt"); // text
    // proxy->storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../../todos.json"); // json

    struct Todo todos[MAX_TODOS] = {0};
    const struct Argument *argument = (struct Argument *) notification->getBody(notification);
    if (strcmp(argument->command.name, "count") == 0) {
        proxy->count(proxy, todos, MAX_TODOS);
    } if (strcmp(argument->command.name, "list") == 0) {
        proxy->list(proxy, todos);
    } else if (strcmp(argument->command.name, "add") == 0) {
        proxy->add(proxy, argument->getOption(argument, "title"), todos);
    } else if (strcmp(argument->command.name, "edit") == 0) {
        const char *id_str = argument->getOption(argument, "id");
        const char *completed_str = argument->getOption(argument, "completed");
        const char *title = argument->getOption(argument, "title");

        unsigned int id = id_str ? (unsigned int) strtoul(id_str, NULL, 10) : 0u;
        bool completed = completed_str && (strcmp(completed_str, "true") == 0 || strcmp(completed_str, "1") == 0);

        proxy->edit(proxy, id, title, completed, todos);
    } else if (strcmp(argument->command.name, "delete") == 0) {
        const char *id_str = argument->getOption(argument, "id");
        unsigned int id = id_str ? (unsigned int) strtoul(id_str, NULL, 10) : 0u;

        proxy->delete(proxy, id, todos);
    } else {
        printf("Unknown Command\n");
    }

    printf("service command end\n");
}

struct ICommand *service_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute; // override
    return command;
}
