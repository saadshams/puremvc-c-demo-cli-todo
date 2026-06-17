#include "service.h"

#include "model/valueObject/todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void run(const struct Service *self, struct Argument *argument) {
    if (self->delegate.onParse != NULL)
        self->delegate.onParse(self->delegate.context, argument);
}

static void result(const struct Service *self, const void *data, const char *path) {
    (void) self;
    path[0] != '\0' ? todo_print(data, path) : printf("%s\n", (char *) data);
}

static void fault(const struct Service *self, enum Status status) {
    (void) self;
    fprintf(stderr, "%s\n", status_message(status));
}

static void setDelegate(struct Service *self, const struct IService delegate) {
    self->delegate = delegate;
}

static size_t size(void) {
    return (sizeof(struct Service) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct Service *alloc() {
    struct Service *service = malloc(size());

    if (service == NULL) {
        fprintf(stderr, "\033[0;31m[Todo::Service::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return service;
}

static struct Service *init(struct Service *this) {
    if (this == NULL) return NULL;

    memset(this, 0, size());

    this->run = run;
    this->result = result;
    this->fault = fault;
    this->setDelegate = setDelegate;

    return this;
}

struct Service *service_new() {
    return init(alloc());
}
