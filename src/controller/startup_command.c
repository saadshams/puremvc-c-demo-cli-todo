#include "startup_command.h"

#include "application_facade.h"
#include "controller/service_command.h"
#include "model/service_proxy.h"
#include "view/service_mediator.h"

static void execute(const struct ICommand *self, struct INotification *notification) {
    const struct INotifier *notifier = self->getNotifier(self);
    const struct IFacade *facade = notifier->getFacade(notifier);

    facade->registerCommand(facade, SERVICE, service_command_new);
    facade->registerProxy(facade, service_proxy_new());
    facade->registerMediator(facade, service_mediator_new(notification->getBody(notification)));
}

struct ICommand *startup_command_new(void) {
    struct ICommand *command = puremvc_command_new();
    command->execute = execute; // override
    return command;
}
