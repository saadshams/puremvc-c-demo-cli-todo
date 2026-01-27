#include "../application_facade.h"
#include "cli_mediator.h"

static void onParse(const struct IMediator *self, struct Command *command) {
    const char *error = NULL;
    const struct IFacade *facade = self->notifier->getFacade(self->notifier, &error);
    facade->sendNotification(facade, SERVICE, command, NULL, &error);
}

static void onRegister(struct IMediator *self) {
    const char *error = NULL;
    struct CLI *cli = self->getComponent(self);
    cli->setDelegate(cli, self, (void (*)(const void *, struct Command *)) onParse);
    cli->parse(cli, &error);
}

static const char **listNotificationInterests(const struct IMediator *self, const char **error) {
    static const char *interests[] = { NULL };
    return interests;
}

struct IMediator *todo_cli_mediator_new(struct CLI *component, const char **error) {
    struct IMediator *mediator = puremvc_mediator_new(CLIMediator_NAME, component, error);
    mediator->onRegister = onRegister;
    mediator->listNotificationInterests = listNotificationInterests;
    return mediator;
}
