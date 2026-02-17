#include "service_command.h"
#include "../model/service_proxy.h"
#include "../model/storage/todo_text.h"

#include <stdio.h>

static void execute(const struct ICommand *self, struct INotification *notification) {
    printf("service command\n");

    const struct IFacade *facade = self->getNotifier(self)->getFacade(self->getNotifier(self));

    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);

    // Strategy Pattern, Dependency Injection
    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    proxy->storage = storage;

    proxy->list(proxy->super, notification->getBody(notification));

    printf("service command end\n");
}

struct ICommand *service_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute; // override
    return command;
}
