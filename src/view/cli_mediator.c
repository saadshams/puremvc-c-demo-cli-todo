#include "cli_mediator.h"

#include <stdio.h>

static void onParse(struct IMediator *self, struct Command *command) {
    printf("on parse");
}

static void onRegister(struct IMediator *self) {
    const char *error = NULL;
    struct CLI *cli = self->getComponent(self);
    cli->setDelegate(cli, self, (void (*)(void *, struct Command *))onParse);
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
