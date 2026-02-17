#include "../application_facade.h"
#include "service_mediator.h"

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
    static const char *interests[] = { NULL };
    return interests;
}

struct IMediator *service_mediator_init(void *buffer, const char *name, void *component) {
    struct IMediator *mediator = puremvc_mediator_init(buffer, name, component);
    mediator->onRegister = onRegister;
    mediator->listNotificationInterests = listNotificationInterests;
    return mediator;
}
