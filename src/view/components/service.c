#include "service.h"
#include "model/valueObject/todo.h"

#include <stdio.h>
#include <string.h>

static void start(const struct Service *self, const int argc, char **argv) {
    if (argc < 2) {
        self->argument->options[self->argument->count].name = "-h";
        if (self->delegate.onParse != NULL)
            self->delegate.onParse(self->delegate.context, self->argument);
        return;
    }

    int i = 1;

    if (i < argc && argv[i][0] != '-') {
        self->argument->command.name = argv[i];
        i++;
    }

    if (i < argc && argv[i][0] != '-') {
        self->argument->command.value = argv[i];
        i++;
    }

    while (i < argc && self->argument->count < MAX_OPTIONS) {
        if (argv[i][0] == '-') {
            self->argument->options[self->argument->count].name = argv[i];
            if (i + 1 < argc && argv[i + 1][0] != '-' ) {
                self->argument->options[self->argument->count].value = argv[i + 1];
                self->argument->count++;
                i += 2;
            } else {
                self->argument->options[self->argument->count].value = "true";
                self->argument->count++;
                i += 1;
            }
        }
    }

    if (i < argc) {
        self->argument->extra = argv[i];
    }

    if (self->delegate.onParse != NULL)
        self->delegate.onParse(self->delegate.context, self->argument);
}

static void result(const struct Service *self, const void *data, const char *type) {
    (void) self;
    type[0] != '\0' ? todo_print(data, type) : printf("%s\n", (char *) data);
}

static void fault(const struct Service *self, enum Status status) {
    (void) self;
    fprintf(stderr, "%s\n", status_message(status));
}

static void setDelegate(struct Service *self, const struct IService delegate) {
    self->delegate = delegate;
}

struct Service *service_init(struct Service *self, struct Argument *argument) {
    self->start = start;
    self->result = result;
    self->fault = fault;
    self->setDelegate = setDelegate;

    self->argument = argument;

    return self;
}
