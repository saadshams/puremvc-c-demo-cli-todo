#include <stdlib.h>
#include <string.h>

#include "service.h"

static void todo_parseObject(const struct Service *self) {
    // todo pass the error to command
    // if (self->argc < 2) return *error = "Usage: todo <command> [options]", (void)0;

    int i = 1;

    if (i < self->argc && self->argv[i][0] != '-') {
        self->command->subCommand.name = self->argv[i];
        i++;
    }

    if (i < self->argc && self->argv[i][0] != '-') {
        self->command->subCommand.value = self->argv[i];
        i++;
    }

    while (i < self->argc && self->command->count < MAX_OPTIONS) {
        if (self->argv[i][0] == '-') {
            self->command->options[self->command->count].name = self->argv[i];
            if (i + 1 < self->argc && self->argv[i + 1][0] != '-' ) {
                self->command->options[self->command->count].value = self->argv[i + 1];
                self->command->count++;
                i += 2;
            } else {
                self->command->options[self->command->count].value = "true";
                self->command->count++;
                i += 1;
            }
        }
    }

    if (i < self->argc) {
        self->command->extra = self->argv[i];
    }

    if (self->delegate != NULL)
        self->delegate(self->context, self->command);
}

static void setDelegate(struct Service *self, const void *context, void (*delegate)(const void *context, struct Argument *command)) {
    self->context = context;
    self->delegate = delegate;
}

void service_init(struct Service *service, struct Argument *command, int argc, char **argv) {
    memset(service, 0, sizeof(*service));

    service->command = command;
    service->argc = argc;
    service->argv = argv;

    service->parse = todo_parseObject;
    service->setDelegate = setDelegate;
}
