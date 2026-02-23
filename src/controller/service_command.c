#include "service_command.h"

#include "application_facade.h"
#include "model/service_proxy.h"
#include "model/valueObject/argument.h"
#include "model/storage/text.h"
#include "model/storage/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void execute(const struct ICommand *self, struct INotification *notification) {

    const struct INotifier *notifier = self->getNotifier(self);
    const struct IFacade *facade = notifier->getFacade(notifier);
    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);

    const struct Argument *argument = notification->getBody(notification);
    if (argument->getFlag(argument, "--version") || argument->getFlag(argument, "-v")) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->version(proxy), NULL);
        return;
    }
    if (argument->getFlag(argument, "--help") || argument->getFlag(argument, "-h")) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->help(proxy), NULL);
        return;
    }

    enum Status status = ERR_INVALID_ARGS;

    // Strategy Pattern, Dependency Injection
    const char *path = argument->getFlag(argument, "--file") ? argument->getFlag(argument, "--file") : argument->getFlag(argument, "-f");
    if (path == NULL) {
        facade->sendNotification(facade, SERVICE_FAULT, (void *)(intptr_t) ERR_FILE_NOT_FOUND, "");
        return;
    }

    const char *extension = strrchr(path, '.'); // Find the last dot
    const char *strategy = extension && strcmp(extension, ".json") == 0 ? "json" : "text";
    if (strcmp(strategy, "json") == 0) {
        proxy->storage = todo_json_storage_init(alloca(todo_json_storage_size()), path);
    } else {
        proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), path);
    }

    // main commands
    struct Todo todos[MAX_TODOS] = {0};
    if (strcmp(argument->command.name, "list") == 0) {
        status = proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "add") == 0) {
        status = proxy->add(proxy, argument->command.value);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "edit") == 0) {
        const char *id_str = argument->command.value;
        const char *completed_str = argument->getFlag(argument, "-c") ? argument->getFlag(argument, "-c") : argument->getFlag(argument, "--completed");
        const char *title = argument->getFlag(argument, "-t") ? argument->getFlag(argument, "-t") : argument->getFlag(argument, "--title");
        const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;
        const bool completed = completed_str != NULL && strcmp(completed_str, "true") == 0;

        status = proxy->edit(proxy, id, title, completed);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    } else if (strcmp(argument->command.name, "delete") == 0) {
        const char *id_str = argument->command.value;
        const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;

        status = id == 0 ? ERR_INVALID_ARGS : proxy->delete(proxy, id);
        if (status == OK) proxy->list(proxy, todos, MAX_TODOS);
    }

    if (status == ERR_INVALID_ARGS) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->help(proxy), NULL);
    } else if (status == OK) {
        facade->sendNotification(facade, SERVICE_RESULT, todos, strategy);
    } else {
        facade->sendNotification(facade, SERVICE_FAULT, (void *)(intptr_t) status, "");
    }
}

struct ICommand *service_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute; // override
    return command;
}
