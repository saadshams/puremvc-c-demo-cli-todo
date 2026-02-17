#include "argument.h"

#include <string.h>

static const char *getOption(const struct Argument *self, const char *name) {
    if (name == NULL) return NULL;

    for (int i = 0; i < self->count; i++) { // search
        if (self->options[i].name != NULL && strcmp(self->options[i].name, name) == 0) { // match
            return self->options[i].value;
        }
    }

    return NULL;
}

struct Argument *todo_argument_init(void *buffer) {
    struct Argument *argument = buffer;
    argument->getOption = getOption;
    return argument;
}
