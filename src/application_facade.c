#include "application_facade.h"

#include "controller/startup_command.h"

static void (*super_initializeController)(struct IFacade *self);

static void initializeController(struct IFacade *self) {
    super_initializeController(self); // Call overridden (super) initialization
    self->registerCommand(self, STARTUP, startup_command_new);
}

static void startup(const struct ApplicationFacade *self, struct Service *service) {
    const struct IFacade *facade = self->super;
    facade->sendNotification(facade, STARTUP, service, NULL);
}

struct IFacade *application_facade_getInstance(const char *key) {
    struct IFacade *facade = puremvc_facade_getInstance(key);
    super_initializeController = facade->initializeController; // Save original initializer
    facade->initializeController = initializeController; // override
    return facade;
}

struct ApplicationFacade *application_facade_extend(struct ApplicationFacade *facade, struct IFacade *super) {
    facade->super = super;
    facade->startup = startup;

    return facade;
}
