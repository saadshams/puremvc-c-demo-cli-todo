#include "service_command.h"

#include "../application_facade.h"
#include "../model/service_proxy.h"
#include "../model/valueObject/argument.h"
#include "../model/storage/todo_text.h"
#include "../model/storage/todo_json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void execute(const struct ICommand *self, struct INotification *notification) {

    const struct IFacade *facade = self->getNotifier(self)->getFacade(self->getNotifier(self));
    struct IProxy *super = facade->retrieveProxy(facade, ServiceProxy_NAME);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);

    // Strategy Pattern, Dependency Injection
    const char *type = "json"; // text|json
    if (strcmp(type, "text") == 0) {
        proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../todos.txt");
    } else {
        proxy->storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../todos.json");
    }

    struct Todo todos[MAX_TODOS] = {0};
    const struct Argument *argument = notification->getBody(notification);

    // options
    if (argument->options[0].name != NULL && strcmp(argument->options[0].name, "--version") == 0) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->version(proxy), NULL);
        return;
    }

    if (argument->options[0].name != NULL && strcmp(argument->options[0].name, "--help") == 0) {
        facade->sendNotification(facade, SERVICE_RESULT, (void *) proxy->help(proxy), NULL);
        return;
    }

    // main commands
    bool success = false;
    if (strcmp(argument->command.name, "list") == 0) {
        proxy->list(proxy, todos, MAX_TODOS);
        success = true;
    } else if (strcmp(argument->command.name, "add") == 0) {
        proxy->add(proxy, argument->getOption(argument, "title"));
        proxy->list(proxy, todos, MAX_TODOS);
        success = true;
    } else if (strcmp(argument->command.name, "edit") == 0) {
        const char *id_str = argument->getOption(argument, "id");
        const char *completed_str = argument->getOption(argument, "completed");
        const char *title = argument->getOption(argument, "title");

        const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;
        const bool completed = completed_str && (strcmp(completed_str, "true") == 0 || strcmp(completed_str, "1") == 0);

        proxy->edit(proxy, id, title, completed);
        proxy->list(proxy, todos, MAX_TODOS);
        success = true;
    } else if (strcmp(argument->command.name, "delete") == 0) {
        const char *id_str = argument->getOption(argument, "id");
        const unsigned int id = id_str ? strtoul(id_str, NULL, 10) : 0u;

        proxy->delete(proxy, id);
        proxy->list(proxy, todos, MAX_TODOS);
        success = true;
    }

    if (success == true) {
        facade->sendNotification(facade, SERVICE_RESULT, todos, strcmp(type, "text") == 0 ? "text" : "json");
    } else {
        facade->sendNotification(facade, SERVICE_FAULT, "[CLIDemo::ServiceCommand::execute] Error: Unknown command - Valid commands: list, add, edit, delete.", "");
    }
}

struct ICommand *service_command_init(void *buffer) {
    struct ICommand *command = puremvc_simple_command_init(buffer);
    command->execute = execute; // override
    return command;
}
