#include "application_facade.h"
#include "view/components/service.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    const char *name = "todoApp";

    // As per MISRA, all memory will be allocated at the call site and provided to the Framework for management

    // 1. Allocate Model (ProxyMap)
    struct ModelMap **modelMap = (struct ModelMap *[]) { &(struct ModelMap){ .model = alloca(puremvc_model_size()) }, NULL };
    struct ProxyMap **proxyMap = (struct ProxyMap *[]) { &(struct ProxyMap){ .proxy = alloca(puremvc_proxy_size()) }, NULL }; // Proxy: ServiceProxy
    struct IModel *model = puremvc_model_getInstance(modelMap, name);
    model->initializeModel(model, proxyMap);

    // 2. Allocate View (ObserverMap, MediatorMap)
    struct ViewMap **viewMap = (struct ViewMap *[]) { &(struct ViewMap){ .view = alloca(puremvc_view_size()) }, NULL };
    struct ObserverMap **observerMap = (struct ObserverMap *[]) { // Notifications with Observers: STARTUP, SERVICE, SERVICE_RESULT
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // STARTUP
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // SERVICE
        NULL
    };
    struct MediatorMap **mediatorMap = (struct MediatorMap *[]) { &(struct MediatorMap){ .mediator = alloca(puremvc_mediator_size()) }, NULL }; // Mediator: ServiceMediator
    struct IView *view = puremvc_view_getInstance(viewMap, name);
    view->initializeView(view, observerMap, mediatorMap);

    // 3. Allocate Controller (CommandMap)
    struct ControllerMap **controllerMap = (struct ControllerMap *[]) { &(struct ControllerMap){ . controller = alloca(puremvc_controller_size()) }, NULL };
    struct CommandMap **commandMap = (struct CommandMap *[]) { &(struct CommandMap){}, &(struct CommandMap){}, NULL }; // Command: StartupCommand, ServiceCommand
    struct IController *controller = puremvc_controller_getInstance(controllerMap, name);
    controller->initializeController(controller, view, commandMap);

    // 4. Allocate Facade
    struct FacadeMap **facadeMap = (struct FacadeMap *[]) { &(struct FacadeMap){ .facade = alloca(puremvc_facade_size()) }, NULL };
    struct IFacade *super = application_facade_getInstance(facadeMap, name);
    super->initializeFacade(super, model, view, controller);

    // 5. Link ApplicationFacade's super to Facade
    const struct ApplicationFacade *facade = application_facade_bind(&(struct ApplicationFacade){}, super);

    // 5. Allocate Service Component
    struct Service *service = &(struct Service) {};
    struct Command *command = &(struct Command) {};
    service_init(service, command, argc, argv);

    // 6. PureMVC Apparatus Startup and Link Service Component through a Mediator
    facade->startup(facade->super, service);

    // 7. Start Service Component
    service->parse(service);

    return 0;
}
