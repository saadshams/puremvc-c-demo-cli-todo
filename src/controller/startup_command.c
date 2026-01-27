#include "startup_command.h"

#include "../model/service_proxy.h"
#include "../view/cli_mediator.h"

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    struct CLI *cli = (struct CLI *) notification->getBody(notification);

    const struct IFacade *facade = self->notifier->getFacade(self->notifier, error);

    facade->registerProxy(facade, todo_service_proxy_new(error), error);
    facade->registerMediator(facade, todo_cli_mediator_new(cli, error), error);
}

struct ICommand *todo_startup_command_new(const char **error) {
    struct ICommand *command = puremvc_simple_command_new(error);
    command->execute = execute; // override
    return command;
}
