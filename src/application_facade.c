#include "application_facade.h"

#include "controller/startup_command.h"

static void (*super_initializeController)(struct IFacade *self, struct IController *controller);

static void initializeController(struct IFacade *self, struct IController *controller) {
    super_initializeController(self, controller); // call super
    self->registerCommand(self, STARTUP, startup_command_init);
}

static void startup(struct IFacade *self, struct Service *service) {
    self->sendNotification(self, STARTUP, service, NULL);
}

struct IFacade *application_facade_getInstance(struct FacadeMap **facadeMap, const char *key) {
    struct IFacade *facade = puremvc_facade_getInstance(facadeMap, key);
    super_initializeController = facade->initializeController;
    facade->initializeController = initializeController; // override
    return facade;
}

struct ApplicationFacade *application_facade_bind(struct ApplicationFacade *facade, struct IFacade *super) {
    facade->super = super;
    facade->startup = startup;

    return facade;
}
