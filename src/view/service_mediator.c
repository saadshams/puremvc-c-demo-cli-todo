#include "service_mediator.h"

#include "application_facade.h"
#include "view/components/service.h"

#include <string.h>
#include <stdio.h>

static void onParse(const struct IMediator *self, void *data) {
    const struct INotifier *notifier = self->getNotifier(self);
    const struct IFacade *facade = notifier->getFacade(notifier);
    facade->sendNotification(facade, SERVICE, data, NULL);
}

static void onRegister(struct IMediator *self) {
    struct Service *service = self->getComponent(self);
    // Register this Mediator as the service callback target.
    // The service stores the Mediator in the generic context pointer (void *)
    // to maintain loose coupling between the Service and Mediator.
    service->setDelegate(service, (struct IService){ .context = self, .onParse = (void (*) (void *, void *)) onParse });
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
        service->fault(service, (enum Status)(intptr_t) notification->getBody(notification));
    } else {
        fprintf(stderr, "\033[0;31m[Todo::ServiceMediator::handleNotification] ERROR: Unknown notification.\033[0m\n");
    }
}

struct IMediator *service_mediator_new(void *component) {
    struct IMediator *mediator = puremvc_mediator_new(ServiceMediator_NAME, component);
    mediator->onRegister = onRegister;
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}

struct ServiceMediator *service_mediator_extend(struct ServiceMediator *mediator, struct IMediator *super) {
    mediator->super = super;
    return mediator;
}
