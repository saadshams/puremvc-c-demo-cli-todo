#include "startup_command.h"

#include "../model/service_proxy.h"
#include "../view/service_mediator.h"

static void execute(const struct ICommand *self, struct INotification *notification) {
    const struct IFacade *facade = self->getNotifier(self)->getFacade(self->getNotifier(self));

    facade->registerProxy(facade, service_proxy_init, ServiceProxy_NAME, NULL);
    facade->registerMediator(facade, service_mediator_init, ServiceMediator_NAME, notification->getBody(notification));
}

struct ICommand *startup_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute; // override
    return command;
}
