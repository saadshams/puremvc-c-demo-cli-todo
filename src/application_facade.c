#include "application_facade.h"

#include "controller/service_command.h"
#include "controller/startup_command.h"

static void (*initializeController_)(struct IFacade *self, const char **error);

static void initializeController(struct IFacade *self, const char **error) {
    initializeController_(self, error); // base
    self->registerCommand(self, STARTUP, todo_startup_command_new, error);
    self->registerCommand(self, SERVICE, todo_service_command_new, error);
}

struct IFacade *todo_facade_getInstance(const char *key, const char **error) {
    struct IFacade *facade = puremvc_facade_getInstance(key, puremvc_facade_new, error);
    initializeController_ = facade->initializeController; // store
    facade->initializeController = initializeController; // override
    return facade;
}

void todo_startup(const struct IFacade *self, struct CLI *cli, const char **error) {
    self->sendNotification(self, STARTUP, cli, NULL, error);
}
