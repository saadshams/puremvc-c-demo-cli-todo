#include "service.h"

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

static void result(const struct Service *self, const struct Todo *todos, const char *type) {
    (void) self;

    size_t count = 0u;
    while (todos[count].id != 0u) count++;

    if (strcmp(type, "text") == 0) {
        for (size_t i = 0u; i < count; i++) {
            printf("%u | %s | %s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false");
        }
    } else if (strcmp(type, "json") == 0) {
        printf("[\n");
        for (size_t i = 0u; todos[i].id != 0; i++) {
            printf("\t{ \"id\": %u, \"title\": %s, \"completed\": %s }%s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", i + 1 < count ? "," : "");
        }
        printf("]");
    } else {
        self->fault(self, "[CLIDemo::Service::result] Error: Unknown type.\n");
    }
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
