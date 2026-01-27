#include "service_command.h"

#include <stdio.h>

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    printf("service execute");
}

struct ICommand *todo_service_command_new(const char **error) {
    struct ICommand *command = puremvc_simple_command_new(error);
    command->execute = execute;
    return command;
}
