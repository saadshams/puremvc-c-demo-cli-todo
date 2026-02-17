#include "service_mediator.h"

#include "../application_facade.h"

#include <stdio.h>
#include <string.h>

static void onParse(const struct IMediator *self, struct Argument *command) {
    const struct INotifier *notifier = self->getNotifier(self);
    const struct IFacade *facade = notifier->getFacade(notifier);
    facade->sendNotification(facade, SERVICE, command, NULL);
}

static void onRegister(struct IMediator *self) {
    struct Service *service = self->getComponent(self);
    service->setDelegate(service, self, (void (*)(const void *, struct Argument *)) onParse);
}

static const char *const *listNotificationInterests(const struct IMediator *self) {
    static const char *interests[] = { SERVICE_RESULT, SERVICE_FAULT, NULL };
    return interests;
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    const struct Service *service = self->getComponent(self);
    if (strcmp(notification->getName(notification), SERVICE_RESULT) == 0) {
        service->result(service, notification->getBody(notification), notification->getType(notification));
    } else if (strcmp(notification->getName(notification), SERVICE_FAULT) == 0) {
        service->fault(service, notification->getBody(notification));
    } else {
        fprintf(stderr, "[CLIDemo::ServiceMediator::handleNotification] Error: Unknown notification.\n");
    }
}

struct IMediator *service_mediator_init(void *buffer, const char *name, void *component) {
    struct IMediator *mediator = puremvc_mediator_init(buffer, name, component);
    mediator->onRegister = onRegister;
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}
