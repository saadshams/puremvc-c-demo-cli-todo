#include "application_facade.h"

#include "controller/startup_command.h"

static void (*super_initializeController)(struct IFacade *self, struct IController *controller);

static void initializeController(struct IFacade *self, struct IController *controller) {
    super_initializeController(self, controller); // Call original (super) initialization
    self->registerCommand(self, STARTUP, startup_command_init);
}

static void startup(const struct ApplicationFacade *self, struct Service *service) {
    const struct IFacade *facade = self->super;
    facade->sendNotification(facade, STARTUP, service, NULL);
}

struct IFacade *application_facade_getInstance(struct FacadeMap **facadeMap, const char *key) {
    struct IFacade *facade = puremvc_facade_getInstance(facadeMap, key);
    super_initializeController = facade->initializeController; // Save original initializer
    facade->initializeController = initializeController; // override
    return facade;
}

struct ApplicationFacade *application_facade_bind(struct ApplicationFacade *facade, struct IFacade *super) {
    facade->super = super;
    facade->startup = startup;

    return facade;
}
