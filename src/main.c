#include "application_facade.h"
#include "view/components/service.h"

#include <stdio.h>
#include <string.h>

int main(const int argc, char **argv) {
    const char *name = "todoApp";

    // Application bootstrap and PureMVC initialization
    //
    // Memory Management & MISRA Compliance:
    //   - All memory is allocated at the call site using stack allocation (alloca) and provided to the framework.
    //   - No dynamic heap allocation is performed; the framework manages only the memory given to it.
    //   - If your application has more commands, mediators, or proxies, simply add slots to the respective maps.
    //
    // PureMVC Initialization Flow:
    // 1. Model: Allocates ModelMap and ProxyMap (ServiceProxy), then initializes the IModel instance.
    // 2. View: Allocates ViewMap, ObserverMap (for notifications STARTUP, SERVICE, SERVICE_RESULT, SERVICE_FAULT),
    //    and MediatorMap (ServiceMediator), then initializes the IView instance.
    // 3. Controller: Allocates ControllerMap and CommandMap (StartupCommand, ServiceCommand), then initializes IController.
    // 4. Facade: Allocates FacadeMap and initializes the IFacade instance with Model, View, and Controller.
    // 5. ApplicationFacade: Binds a concrete ApplicationFacade to the generic Facade instance.
    // 6. Service Component: Allocates the Service structure, initializes Argument parser, and sets up the service
    //    with argc/argv passed from main.
    // 7. Start Service Component: Calls service->parse(service) to process command-line arguments and trigger execution.

    // 1. Allocate Model and ProxyMap
    struct ModelMap **modelMap = (struct ModelMap *[]) {
        &(struct ModelMap){ .model = alloca(puremvc_model_size()) }, // Model instance (Multiton Key)
        NULL
    };
    struct ProxyMap **proxyMap = (struct ProxyMap *[]) {
        &(struct ProxyMap){ .proxy = alloca(puremvc_proxy_size()) }, // ServiceProxy
        NULL
    };
    struct IModel *model = puremvc_model_getInstance(modelMap, name);
    model->initializeModel(model, proxyMap); // Link Model with its Proxies

    // 2. Allocate View with ObserverMap and MediatorMap
    struct ViewMap **viewMap = (struct ViewMap *[]) {
        &(struct ViewMap){ .view = alloca(puremvc_view_size()) }, // View instance (Multiton Key)
        NULL
    };
    struct ObserverMap **observerMap = (struct ObserverMap *[]) { // ObserverMap for Notifications
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // STARTUP Notification Observers
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // SERVICE Notification Observers
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // SERVICE_RESULT Notification Observers
        &(struct ObserverMap) { .observers = (struct IObserver *[]) { memset(alloca(puremvc_observer_size()), 0, puremvc_observer_size()), NULL } }, // SERVICE_FAULT Notification Observers
        NULL
    };
    struct MediatorMap **mediatorMap = (struct MediatorMap *[]) {
        &(struct MediatorMap){ .mediator = alloca(puremvc_mediator_size()) }, // ServiceMediator
        NULL
    };
    struct IView *view = puremvc_view_getInstance(viewMap, name);
    view->initializeView(view, observerMap, mediatorMap); // Link View with Observers and Mediators

    // 3. Allocate Controller (CommandMap)
    struct ControllerMap **controllerMap = (struct ControllerMap *[]) {
        &(struct ControllerMap){ . controller = alloca(puremvc_controller_size()) }, // Controller instance (Multiton Key)
        NULL
    };
    struct CommandMap **commandMap = (struct CommandMap *[]) {
        &(struct CommandMap){}, // StartupCommand
        &(struct CommandMap){}, // ServiceCommand
        NULL
    };
    struct IController *controller = puremvc_controller_getInstance(controllerMap, name);
    controller->initializeController(controller, view, commandMap); // Compose Controller with View and Commands

    // 4. Allocate Facade
    struct FacadeMap **facadeMap = (struct FacadeMap *[]) {
        &(struct FacadeMap){ .facade = alloca(puremvc_facade_size()) }, // Facade instance (Multiton Key)
        NULL
    };
    struct IFacade *super = application_facade_getInstance(facadeMap, name);
    super->initializeFacade(super, model, view, controller); // Compose Facade with Model, View, Controller

    // 5. Init ApplicationFacade and bind its super to Facade
    const struct ApplicationFacade *facade = application_facade_bind(&(struct ApplicationFacade){}, super);

    // 5. Allocate Service Component
    struct Argument *argument = todo_argument_init(&(struct Argument) {}); // CLI Argument
    struct Service *service = service_init(&(struct Service){}, argument); // Argument Parser, Result and Fault handler

    // 6. PureMVC Apparatus Startup and Link Service Component through ServiceMediator
    facade->startup(facade, service);

    // 7. Run Service Component
    service->run(service, argc, argv); // Execute command parsing & dispatch SERVICE notification

    return 0;
}
