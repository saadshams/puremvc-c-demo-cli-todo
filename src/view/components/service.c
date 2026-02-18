#include "service.h"
#include "../../model/valueObject/todo.h"

#include <stdio.h>
#include <string.h>

static void parse(const struct Service *self) {
    if (self->argc < 2) {
        fprintf(stderr, "[CLIDemo::Service::result] Error: Usage: todo <command> [options].\n");
        return;
    }

    int i = 1;

    if (i < self->argc && self->argv[i][0] != '-') {
        self->argument->command.name = self->argv[i];
        i++;
    }

    if (i < self->argc && self->argv[i][0] != '-') {
        self->argument->command.value = self->argv[i];
        i++;
    }

    while (i < self->argc && self->argument->count < MAX_OPTIONS) {
        if (self->argv[i][0] == '-') {
            self->argument->options[self->argument->count].name = self->argv[i];
            if (i + 1 < self->argc && self->argv[i + 1][0] != '-' ) {
                self->argument->options[self->argument->count].value = self->argv[i + 1];
                self->argument->count++;
                i += 2;
            } else {
                self->argument->options[self->argument->count].value = "true";
                self->argument->count++;
                i += 1;
            }
        }
    }

    if (i < self->argc) {
        self->argument->extra = self->argv[i];
    }

    if (self->delegate != NULL)
        self->delegate(self->context, self->argument);
}

static void result(const struct Service *self, const void *data, const char *type) {
    (void) self;
    type != NULL ? todo_print(data, type) : printf("%s\n", (char *) data);
}

static void fault(const struct Service *self, const char *message) {
    (void) self;
    fprintf(stderr, "%s\n", message);
}

static void setDelegate(struct Service *self, const void *context, void (*delegate)(const void *context, struct Argument *argument)) {
    self->context = context;
    self->delegate = delegate;
}

void service_init(struct Service *service, struct Argument *argument, int argc, char **argv) {
    memset(service, 0, sizeof(*service));

    service->argument = argument;
    service->argc = argc;
    service->argv = argv;

    service->parse = parse;
    service->result = result;
    service->fault = fault;
    service->setDelegate = setDelegate;
}
