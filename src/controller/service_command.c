#include "service_command.h"

#include "application_facade.h"
#include "model/service_proxy.h"
#include "model/data/storage.h"
#include "model/valueObject/argument.h"

#include <stdlib.h>
#include <string.h>

static void execute(const struct ICommand *self, struct INotification *notification) {

    const struct INotifier *notifier = self->getNotifier(self);
    const struct IFacade *facade = notifier->getFacade(notifier);

    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    struct ServiceProxy *proxy = service_proxy_extend(&(struct ServiceProxy){}, super);

    const struct Argument *argument = notification->getBody(notification);

    if (argument->getFlag(argument, "--version") || argument->getFlag(argument, "-v")) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->version(proxy), NULL);
        return;
    }

    if (argument->getFlag(argument, "--help") || argument->getFlag(argument, "-h")) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->help(proxy), NULL);
        return;
    }

    const char *path = argument->getFlag(argument, "--file") ? argument->getFlag(argument, "--file") : argument->getFlag(argument, "-f");
    if (path == NULL) {
        facade->sendNotification(facade, SERVICE_FAULT, (void *)(intptr_t) ERR_FILE_NOT_FOUND, "");
        return;
    }

    proxy->storage = storage_new(path);

    // main commands
    struct Todo todos[MAX_TODOS] = {0};
    enum Status status = ERR_INVALID_ARGS;

    if (strcmp(argument->command.name, "list") == 0) {
        status = proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "add") == 0) {
        status = proxy->add(proxy, argument);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "edit") == 0) {
        status = proxy->edit(proxy, argument);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "delete") == 0) {
        status = proxy->delete(proxy, argument);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    }

    // result/fault
    if (status == ERR_INVALID_ARGS) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->help(proxy), NULL);
    } else if (status == OK) {
        facade->sendNotification(facade, SERVICE_RESULT, todos, path);
    } else {
        facade->sendNotification(facade, SERVICE_FAULT, (void *)(intptr_t) status, NULL);
    }

    storage_dealloc(&proxy->storage);
}

struct ICommand *service_command_new(void) {
    struct ICommand *command = puremvc_command_new();
    command->execute = execute; // override
    return command;
}
