#include <stdio.h>

#include "service_command.h"
#include "../model/service_proxy.h"

static void execute(const struct ICommand *self, struct INotification *notification) {
    printf("service command\n");

    const struct IFacade *facade = self->getNotifier(self)->getFacade(self->getNotifier(self));

    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    const struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);
    proxy->list(proxy->super, notification->getBody(notification));

    printf("service command end\n");
}

struct ICommand *service_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute;
    return command;
}
