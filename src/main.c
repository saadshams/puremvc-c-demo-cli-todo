#include <stdlib.h>

#include "application_facade.h"
#include "view/components/service.h"

int main(const int argc, char **argv) {
    struct IFacade *super = application_facade_getInstance("todo");
    super->initializeFacade(super);

    // Init ApplicationFacade and bind its super to Facade
    const struct ApplicationFacade *facade = application_facade_extend(&(struct ApplicationFacade){}, super);

    // PureMVC Apparatus Startup and Link Service Component through ServiceMediator
    struct Service *service = service_new(); // Service Runner, Result and Fault handler
    facade->startup(facade, service);

    // CLI Argument Parser
    struct Argument *argument = argument_new(argc, argv);

    // Dispatch SERVICE notification
    service->run(service, argument);

    free(argument);
    free(service);
    puremvc_facade_dealloc(&super);

    return 0;
}
